#include "riscv_backend.h"

#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

namespace riscv {
namespace {

using llvmir::BinaryOp;
using llvmir::CmpOp;
using llvmir::Function;
using llvmir::FunctionSignature;
using llvmir::Instruction;
using llvmir::InstructionKind;
using llvmir::Module;
using llvmir::Type;
using llvmir::TypeKind;
using llvmir::TypedValue;
using llvmir::ValueKind;

struct Slot {
    size_t offset;
    size_t size;
    size_t align;
};

static size_t alignTo(size_t value, size_t align) {
    return (value + align - 1) / align * align;
}

static int log2Align(size_t align) {
    int result = 0;
    while ((1ULL << result) < align) {
        ++result;
    }
    return result;
}

static bool fitsImm12(size_t value) {
    return value <= 2047;
}

static string hex32(uint32_t value) {
    ostringstream oss;
    oss << "0x" << hex << value << dec;
    return oss.str();
}

static string joinStrings(const vector<string> &parts, const string &sep) {
    string out;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i != 0) out += sep;
        out += parts[i];
    }
    return out;
}

static string symbolName(const string &name) {
    return llvmir::sanitizeSymbol(name);
}

class FunctionEmitter {
public:
    FunctionEmitter(const Module &moduleRef, const Function &funcRef)
        : module(moduleRef), func(funcRef), outgoingSize(0), nextOffset(0),
          raOffset(0), s0Offset(0), frameSize(0) {}

    string lower() {
        layoutFrame();
        emitFunction();
        return joinStrings(lines, "\n") + "\n";
    }

private:
    const Module &module;
    const Function &func;
    vector<string> lines;
    map<string, Slot> valueSlots;
    map<string, Slot> objectSlots;
    size_t outgoingSize;
    size_t nextOffset;
    size_t raOffset;
    size_t s0Offset;
    size_t frameSize;

    string labelFor(const string &name) const {
        return ".L" + symbolName(func.name) + "_" + symbolName(name);
    }

    string exitLabel() const {
        return ".L" + symbolName(func.name) + "_exit";
    }

    size_t slotSizeForType(const Type &type) const {
        if (type.kind == TypeKind::Pointer || type.kind == TypeKind::I64) return 8;
        if (type.kind == TypeKind::Float || type.kind == TypeKind::I32 || type.kind == TypeKind::I1) return 4;
        if (type.kind == TypeKind::I8) return 1;
        return type.size();
    }

    size_t slotAlignForType(const Type &type) const {
        if (type.kind == TypeKind::I1) return 4;
        return type.align();
    }

    Slot allocate(size_t size, size_t align) {
        nextOffset = alignTo(nextOffset, align);
        Slot slot;
        slot.offset = nextOffset;
        slot.size = size;
        slot.align = align;
        nextOffset += size;
        return slot;
    }

    const FunctionSignature &signatureOf(const string &callee) const {
        map<string, FunctionSignature>::const_iterator it = module.signatures.find(callee);
        if (it == module.signatures.end()) {
            throw runtime_error("Missing function signature: " + callee);
        }
        return it->second;
    }

    bool useFloatRegForArg(const FunctionSignature &sig, size_t argIndex, const Type &type) const {
        if (type.kind != TypeKind::Float) {
            return false;
        }
        if (!sig.is_vararg) {
            return true;
        }
        return argIndex < sig.param_types.size();
    }

    size_t computeCallStackBytes(const Instruction &inst) const {
        const FunctionSignature &sig = signatureOf(inst.callee);
        int intRegs = 0;
        int floatRegs = 0;
        size_t stackBytes = 0;
        for (size_t i = 0; i < inst.args.size(); ++i) {
            bool useFloat = useFloatRegForArg(sig, i, inst.args[i].type);
            if (useFloat) {
                if (floatRegs < 8) {
                    ++floatRegs;
                } else {
                    stackBytes = alignTo(stackBytes, 8);
                    stackBytes += 8;
                }
            } else {
                if (intRegs < 8) {
                    ++intRegs;
                } else {
                    stackBytes = alignTo(stackBytes, 8);
                    stackBytes += 8;
                }
            }
        }
        return stackBytes;
    }

    void layoutFrame() {
        outgoingSize = 0;
        for (size_t i = 0; i < func.instructions.size(); ++i) {
            if (func.instructions[i].kind == InstructionKind::Call) {
                outgoingSize = max(outgoingSize, computeCallStackBytes(func.instructions[i]));
            }
        }
        nextOffset = alignTo(outgoingSize, 8);
        for (size_t i = 0; i < func.signature.param_types.size(); ++i) {
            valueSlots[func.param_names[i]] = allocate(slotSizeForType(func.signature.param_types[i]),
                                                       slotAlignForType(func.signature.param_types[i]));
        }
        for (size_t i = 0; i < func.instructions.size(); ++i) {
            const Instruction &inst = func.instructions[i];
            if (inst.kind == InstructionKind::Alloca) {
                objectSlots[inst.name] = allocate(inst.aux_type.size(), inst.aux_type.align());
                valueSlots[inst.name] = allocate(8, 8);
                continue;
            }
            if (inst.has_result) {
                valueSlots[inst.name] = allocate(slotSizeForType(inst.type), slotAlignForType(inst.type));
            }
        }
        nextOffset = alignTo(nextOffset, 8);
        raOffset = nextOffset;
        s0Offset = raOffset + 8;
        frameSize = alignTo(s0Offset + 8, 16);
    }

    void emit(const string &line) {
        lines.push_back("    " + line);
    }

    void emitLabel(const string &line) {
        lines.push_back(line + ":");
    }

    void emitStackAddress(const string &reg, size_t offset) {
        if (fitsImm12(offset)) {
            emit("addi " + reg + ", sp, " + to_string(offset));
            return;
        }
        emit("li t6, " + to_string(offset));
        emit("add " + reg + ", sp, t6");
    }

    void emitLoadFromSp(const string &inst, const string &reg, size_t offset) {
        if (fitsImm12(offset)) {
            emit(inst + " " + reg + ", " + to_string(offset) + "(sp)");
            return;
        }
        emitStackAddress("t6", offset);
        emit(inst + " " + reg + ", 0(t6)");
    }

    void emitStoreToSp(const string &inst, const string &reg, size_t offset) {
        if (fitsImm12(offset)) {
            emit(inst + " " + reg + ", " + to_string(offset) + "(sp)");
            return;
        }
        emitStackAddress("t6", offset);
        emit(inst + " " + reg + ", 0(t6)");
    }

    void emitAdjustSp(long long delta) {
        if (delta >= -2048 && delta <= 2047) {
            emit("addi sp, sp, " + to_string(delta));
            return;
        }
        emit("li t6, " + to_string(delta));
        emit("add sp, sp, t6");
    }

    void emitSetS0ToFrameTop() {
        if (fitsImm12(frameSize)) {
            emit("addi s0, sp, " + to_string(frameSize));
            return;
        }
        emit("li t6, " + to_string(frameSize));
        emit("add s0, sp, t6");
    }

    void emitLoadSlotInt(const string &reg, const Slot &slot, const Type &type) {
        if (type.kind == TypeKind::Pointer || type.kind == TypeKind::I64) {
            emitLoadFromSp("ld", reg, slot.offset);
        } else if (type.kind == TypeKind::I8) {
            emitLoadFromSp("lb", reg, slot.offset);
        } else {
            emitLoadFromSp("lw", reg, slot.offset);
        }
    }

    void emitStoreSlotInt(const string &reg, const Slot &slot, const Type &type) {
        if (type.kind == TypeKind::Pointer || type.kind == TypeKind::I64) {
            emitStoreToSp("sd", reg, slot.offset);
        } else if (type.kind == TypeKind::I8) {
            emitStoreToSp("sb", reg, slot.offset);
        } else {
            emitStoreToSp("sw", reg, slot.offset);
        }
    }

    void emitLoadSlotFloat(const string &freg, const Slot &slot) {
        emitLoadFromSp("flw", freg, slot.offset);
    }

    void emitStoreSlotFloat(const string &freg, const Slot &slot) {
        emitStoreToSp("fsw", freg, slot.offset);
    }

    const Slot &requireSlot(const string &name) const {
        map<string, Slot>::const_iterator it = valueSlots.find(name);
        if (it == valueSlots.end()) {
            throw runtime_error("Unknown local value: %" + name);
        }
        return it->second;
    }

    void loadIntValue(const string &reg, const TypedValue &value) {
        switch (value.value.kind) {
            case ValueKind::Local:
                emitLoadSlotInt(reg, requireSlot(value.value.name), value.type);
                return;
            case ValueKind::Global:
                emit("la " + reg + ", " + symbolName(value.value.name));
                return;
            case ValueKind::IntImmediate:
                emit("li " + reg + ", " + to_string(value.value.int_value));
                return;
            case ValueKind::Null:
                emit("li " + reg + ", 0");
                return;
            case ValueKind::FloatImmediate:
                throw runtime_error("Float immediate used as integer operand");
        }
    }

    void loadFloatValue(const string &freg, const TypedValue &value) {
        switch (value.value.kind) {
            case ValueKind::Local:
                emitLoadSlotFloat(freg, requireSlot(value.value.name));
                return;
            case ValueKind::FloatImmediate:
                emit("li t6, " + hex32(value.value.float_bits));
                emit("fmv.w.x " + freg + ", t6");
                return;
            default:
                throw runtime_error("Unsupported float operand kind");
        }
    }

    void storeResultInt(const string &name, const Type &type, const string &reg) {
        emitStoreSlotInt(reg, requireSlot(name), type);
    }

    void storeResultFloat(const string &name, const string &freg) {
        emitStoreSlotFloat(freg, requireSlot(name));
    }

    void initParameters() {
        int intRegs = 0;
        int floatRegs = 0;
        size_t stackOffset = 0;
        for (size_t i = 0; i < func.signature.param_types.size(); ++i) {
            const Type &type = func.signature.param_types[i];
            const Slot &slot = requireSlot(func.param_names[i]);
            if (type.kind == TypeKind::Float) {
                if (floatRegs < 8) {
                    emitStoreSlotFloat("fa" + to_string(floatRegs), slot);
                    ++floatRegs;
                } else {
                    emitLoadFromSp("flw", "ft0", frameSize + stackOffset);
                    emitStoreSlotFloat("ft0", slot);
                    stackOffset += 8;
                }
            } else {
                if (intRegs < 8) {
                    emitStoreSlotInt("a" + to_string(intRegs), slot, type);
                    ++intRegs;
                } else {
                    if (type.kind == TypeKind::Pointer || type.kind == TypeKind::I64) {
                        emitLoadFromSp("ld", "t0", frameSize + stackOffset);
                    } else {
                        emitLoadFromSp("ld", "t0", frameSize + stackOffset);
                    }
                    emitStoreSlotInt("t0", slot, type);
                    stackOffset += 8;
                }
            }
        }
    }

    void emitGetElementPtr(const Instruction &inst) {
        loadIntValue("t0", inst.ptr);
        Type current = inst.gep_base_type;
        for (size_t i = 0; i < inst.indices.size(); ++i) {
            loadIntValue("t1", inst.indices[i]);
            size_t step = 0;
            if (i == 0) {
                step = current.size();
            } else {
                if (current.kind != TypeKind::Array) {
                    throw runtime_error("getelementptr indexes non-array type");
                }
                step = current.element->size();
                current = *current.element;
            }
            if (step != 1) {
                emit("li t2, " + to_string(step));
                emit("mul t1, t1, t2");
            }
            emit("add t0, t0, t1");
        }
        storeResultInt(inst.name, inst.type, "t0");
    }

    void storeStackArgInt(const Type &type, const string &reg, size_t offset) {
        if (type.kind == TypeKind::Pointer || type.kind == TypeKind::I64) {
            emitStoreToSp("sd", reg, offset);
        } else {
            emitStoreToSp("sd", reg, offset);
        }
    }

    void storeStackArgFloat(const string &freg, size_t offset) {
        emitStoreToSp("fsw", freg, offset);
    }

    void emitCall(const Instruction &inst) {
        const FunctionSignature &sig = signatureOf(inst.callee);
        int intRegs = 0;
        int floatRegs = 0;
        size_t stackOffset = 0;
        for (size_t i = 0; i < inst.args.size(); ++i) {
            bool useFloat = useFloatRegForArg(sig, i, inst.args[i].type);
            if (useFloat) {
                if (floatRegs < 8) {
                    loadFloatValue("ft0", inst.args[i]);
                    emit("fmv.s fa" + to_string(floatRegs) + ", ft0");
                    ++floatRegs;
                } else {
                    loadFloatValue("ft0", inst.args[i]);
                    storeStackArgFloat("ft0", stackOffset);
                    stackOffset += 8;
                }
            } else {
                if (sig.is_vararg && i >= sig.param_types.size() && inst.args[i].type.kind == TypeKind::Float) {
                    throw runtime_error("Variadic float arguments are not supported by this backend");
                }
                loadIntValue("t0", inst.args[i]);
                if (intRegs < 8) {
                    emit("mv a" + to_string(intRegs) + ", t0");
                    ++intRegs;
                } else {
                    storeStackArgInt(inst.args[i].type, "t0", stackOffset);
                    stackOffset += 8;
                }
            }
        }
        emit("call " + symbolName(inst.callee));
        if (!inst.has_result) {
            return;
        }
        if (inst.type.kind == TypeKind::Float) {
            storeResultFloat(inst.name, "fa0");
        } else {
            storeResultInt(inst.name, inst.type, "a0");
        }
    }

    void emitInstruction(const Instruction &inst) {
        switch (inst.kind) {
            case InstructionKind::Label:
                emitLabel(labelFor(inst.name));
                return;
            case InstructionKind::Alloca:
                emitStackAddress("t0", objectSlots[inst.name].offset);
                storeResultInt(inst.name, inst.type, "t0");
                return;
            case InstructionKind::Store:
                loadIntValue("t0", inst.ptr);
                if (inst.value.type.kind == TypeKind::Float) {
                    loadFloatValue("ft0", inst.value);
                    emit("fsw ft0, 0(t0)");
                } else if (inst.value.type.kind == TypeKind::Pointer || inst.value.type.kind == TypeKind::I64) {
                    loadIntValue("t1", inst.value);
                    emit("sd t1, 0(t0)");
                } else if (inst.value.type.kind == TypeKind::I8) {
                    loadIntValue("t1", inst.value);
                    emit("sb t1, 0(t0)");
                } else {
                    loadIntValue("t1", inst.value);
                    emit("sw t1, 0(t0)");
                }
                return;
            case InstructionKind::Load:
                loadIntValue("t0", inst.ptr);
                if (inst.type.kind == TypeKind::Float) {
                    emit("flw ft0, 0(t0)");
                    storeResultFloat(inst.name, "ft0");
                } else if (inst.type.kind == TypeKind::Pointer || inst.type.kind == TypeKind::I64) {
                    emit("ld t1, 0(t0)");
                    storeResultInt(inst.name, inst.type, "t1");
                } else if (inst.type.kind == TypeKind::I8) {
                    emit("lb t1, 0(t0)");
                    storeResultInt(inst.name, inst.type, "t1");
                } else {
                    emit("lw t1, 0(t0)");
                    storeResultInt(inst.name, inst.type, "t1");
                }
                return;
            case InstructionKind::Binary:
                if (inst.type.kind == TypeKind::Float) {
                    loadFloatValue("ft0", inst.lhs);
                    loadFloatValue("ft1", inst.rhs);
                    if (inst.binary_op == BinaryOp::FAdd) emit("fadd.s ft2, ft0, ft1");
                    if (inst.binary_op == BinaryOp::FSub) emit("fsub.s ft2, ft0, ft1");
                    if (inst.binary_op == BinaryOp::FMul) emit("fmul.s ft2, ft0, ft1");
                    if (inst.binary_op == BinaryOp::FDiv) emit("fdiv.s ft2, ft0, ft1");
                    storeResultFloat(inst.name, "ft2");
                } else {
                    loadIntValue("t0", inst.lhs);
                    loadIntValue("t1", inst.rhs);
                    if (inst.binary_op == BinaryOp::Add) emit("addw t2, t0, t1");
                    if (inst.binary_op == BinaryOp::Sub) emit("subw t2, t0, t1");
                    if (inst.binary_op == BinaryOp::Mul) emit("mulw t2, t0, t1");
                    if (inst.binary_op == BinaryOp::SDiv) emit("divw t2, t0, t1");
                    if (inst.binary_op == BinaryOp::SRem) emit("remw t2, t0, t1");
                    storeResultInt(inst.name, inst.type, "t2");
                }
                return;
            case InstructionKind::Cmp:
                if (inst.lhs.type.kind == TypeKind::Float) {
                    loadFloatValue("ft0", inst.lhs);
                    loadFloatValue("ft1", inst.rhs);
                    if (inst.cmp_op == CmpOp::OEq) emit("feq.s t2, ft0, ft1");
                    if (inst.cmp_op == CmpOp::ONe) { emit("feq.s t2, ft0, ft1"); emit("xori t2, t2, 1"); }
                    if (inst.cmp_op == CmpOp::OLt) emit("flt.s t2, ft0, ft1");
                    if (inst.cmp_op == CmpOp::OGt) emit("flt.s t2, ft1, ft0");
                    if (inst.cmp_op == CmpOp::OLe) emit("fle.s t2, ft0, ft1");
                    if (inst.cmp_op == CmpOp::OGe) emit("fle.s t2, ft1, ft0");
                } else {
                    loadIntValue("t0", inst.lhs);
                    loadIntValue("t1", inst.rhs);
                    switch (inst.cmp_op) {
                        case CmpOp::Eq: emit("xor t2, t0, t1"); emit("seqz t2, t2"); break;
                        case CmpOp::Ne: emit("xor t2, t0, t1"); emit("snez t2, t2"); break;
                        case CmpOp::Slt: emit("slt t2, t0, t1"); break;
                        case CmpOp::Sgt: emit("slt t2, t1, t0"); break;
                        case CmpOp::Sle: emit("slt t2, t1, t0"); emit("xori t2, t2, 1"); break;
                        case CmpOp::Sge: emit("slt t2, t0, t1"); emit("xori t2, t2, 1"); break;
                        default: throw runtime_error("Invalid integer compare op");
                    }
                }
                storeResultInt(inst.name, inst.type, "t2");
                return;
            case InstructionKind::ZExt:
                loadIntValue("t0", inst.value);
                storeResultInt(inst.name, inst.type, "t0");
                return;
            case InstructionKind::Xor:
                loadIntValue("t0", inst.lhs);
                loadIntValue("t1", inst.rhs);
                emit("xor t2, t0, t1");
                storeResultInt(inst.name, inst.type, "t2");
                return;
            case InstructionKind::BrCond:
                loadIntValue("t0", inst.value);
                emit("bne t0, x0, " + labelFor(inst.true_label));
                emit("j " + labelFor(inst.false_label));
                return;
            case InstructionKind::BrUncond:
                emit("j " + labelFor(inst.target_label));
                return;
            case InstructionKind::Call:
                emitCall(inst);
                return;
            case InstructionKind::Bitcast:
                loadIntValue("t0", inst.value);
                storeResultInt(inst.name, inst.type, "t0");
                return;
            case InstructionKind::GetElementPtr:
                emitGetElementPtr(inst);
                return;
            case InstructionKind::Cast:
                if (inst.cast_op == llvmir::CastOp::Sitofp) {
                    loadIntValue("t0", inst.value);
                    emit("fcvt.s.w ft0, t0");
                    storeResultFloat(inst.name, "ft0");
                } else {
                    loadFloatValue("ft0", inst.value);
                    emit("fcvt.w.s t0, ft0, rtz");
                    storeResultInt(inst.name, inst.type, "t0");
                }
                return;
            case InstructionKind::Ret:
                if (inst.type.kind == TypeKind::Void) {
                    emit("j " + exitLabel());
                } else if (inst.type.kind == TypeKind::Float) {
                    loadFloatValue("fa0", inst.value);
                    emit("j " + exitLabel());
                } else {
                    loadIntValue("a0", inst.value);
                    emit("j " + exitLabel());
                }
                return;
        }
    }

    void emitFunction() {
        lines.push_back(".text");
        lines.push_back(".globl " + symbolName(func.name));
        lines.push_back(symbolName(func.name) + ":");
        emitAdjustSp(-static_cast<long long>(frameSize));
        emitStoreToSp("sd", "ra", raOffset);
        emitStoreToSp("sd", "s0", s0Offset);
        emitSetS0ToFrameTop();
        initParameters();
        for (size_t i = 0; i < func.instructions.size(); ++i) {
            emitInstruction(func.instructions[i]);
        }
        emitLabel(exitLabel());
        emitLoadFromSp("ld", "ra", raOffset);
        emitLoadFromSp("ld", "s0", s0Offset);
        emitAdjustSp(static_cast<long long>(frameSize));
        emit("ret");
    }
};

static void emitInitializer(vector<string> &out, const Type &type, const llvmir::Initializer &init) {
    if (init.kind == llvmir::Initializer::Kind::Zero) {
        out.push_back("    .zero " + to_string(type.size()));
        return;
    }
    if (init.kind == llvmir::Initializer::Kind::StringBytes) {
        for (size_t i = 0; i < init.bytes.size(); ++i) {
            out.push_back("    .byte " + to_string(static_cast<int>(init.bytes[i])));
        }
        return;
    }
    if (type.kind == TypeKind::Array) {
        for (size_t i = 0; i < init.elements.size(); ++i) {
            emitInitializer(out, *type.element, init.elements[i]);
        }
        return;
    }
    if (type.kind == TypeKind::Float) {
        out.push_back("    .word " + hex32(init.float_bits));
        return;
    }
    if (type.kind == TypeKind::Pointer || type.kind == TypeKind::I64) {
        out.push_back("    .quad " + to_string(init.int_value));
        return;
    }
    if (type.kind == TypeKind::I8) {
        out.push_back("    .byte " + to_string(init.int_value));
        return;
    }
    out.push_back("    .word " + to_string(init.int_value));
}

static string lowerGlobal(const llvmir::Global &global) {
    vector<string> out;
    bool zero = global.init.kind == llvmir::Initializer::Kind::Zero;
    if (!global.is_constant && zero) {
        out.push_back(".bss");
    } else if (global.is_constant) {
        out.push_back(".section .rodata");
    } else {
        out.push_back(".data");
    }
    out.push_back(".align " + to_string(log2Align(max<size_t>(1, global.type.align()))));
    if (!global.is_private) {
        out.push_back(".globl " + symbolName(global.name));
    }
    out.push_back(symbolName(global.name) + ":");
    emitInitializer(out, global.type, global.init);
    return joinStrings(out, "\n") + "\n";
}

}  // namespace

string lowerModule(const Module &module) {
    vector<string> parts;
    parts.push_back(".attribute arch, \"rv64gc\"");
    for (size_t i = 0; i < module.globals.size(); ++i) {
        parts.push_back(lowerGlobal(module.globals[i]));
    }
    for (size_t i = 0; i < module.functions.size(); ++i) {
        FunctionEmitter emitter(module, module.functions[i]);
        parts.push_back(emitter.lower());
    }
    string out;
    for (size_t i = 0; i < parts.size(); ++i) {
        out += parts[i];
        if (!parts[i].empty() && parts[i][parts[i].size() - 1] != '\n') {
            out += '\n';
        }
    }
    return out;
}

}  // namespace riscv

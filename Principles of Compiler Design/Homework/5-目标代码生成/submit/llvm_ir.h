#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace llvmir {

enum class TypeKind {
    Void,
    I1,
    I8,
    I32,
    I64,
    Float,
    Pointer,
    Array
};

struct Type {
    TypeKind kind;
    int array_length;
    std::shared_ptr<Type> element;

    Type();
    static Type make(TypeKind kind);
    static Type makePointer(const Type &elementType);
    static Type makeArray(int length, const Type &elementType);

    bool operator==(const Type &other) const;
    bool operator!=(const Type &other) const;
    size_t size() const;
    size_t align() const;
    std::string str() const;
};

enum class ValueKind {
    Local,
    Global,
    IntImmediate,
    FloatImmediate,
    Null
};

struct Value {
    ValueKind kind;
    std::string name;
    long long int_value;
    uint32_t float_bits;

    Value();
};

struct TypedValue {
    Type type;
    Value value;
};

enum class BinaryOp {
    Add,
    Sub,
    Mul,
    SDiv,
    SRem,
    FAdd,
    FSub,
    FMul,
    FDiv
};

enum class CmpOp {
    Eq,
    Ne,
    Slt,
    Sgt,
    Sle,
    Sge,
    OEq,
    ONe,
    OLt,
    OGt,
    OLe,
    OGe
};

enum class CastOp {
    Sitofp,
    Fptosi
};

enum class InstructionKind {
    Label,
    Alloca,
    Store,
    Load,
    Binary,
    Cmp,
    ZExt,
    Xor,
    BrCond,
    BrUncond,
    Call,
    Bitcast,
    GetElementPtr,
    Cast,
    Ret
};

struct Instruction {
    InstructionKind kind;
    std::string name;
    Type type;
    Type aux_type;
    BinaryOp binary_op;
    CmpOp cmp_op;
    CastOp cast_op;
    TypedValue lhs;
    TypedValue rhs;
    TypedValue value;
    TypedValue ptr;
    Type gep_base_type;
    std::vector<TypedValue> indices;
    std::vector<TypedValue> args;
    std::string callee;
    bool has_result;
    bool is_vararg;
    std::string true_label;
    std::string false_label;
    std::string target_label;

    Instruction();
};

struct Initializer {
    enum class Kind {
        Zero,
        ScalarInt,
        ScalarFloat,
        Array,
        StringBytes
    };

    Kind kind;
    long long int_value;
    uint32_t float_bits;
    std::vector<Initializer> elements;
    std::vector<unsigned char> bytes;

    Initializer();
};

struct Global {
    std::string name;
    Type type;
    bool is_constant;
    bool is_private;
    Initializer init;
};

struct FunctionSignature {
    Type return_type;
    std::vector<Type> param_types;
    bool is_vararg;

    FunctionSignature();
};

struct Function {
    std::string name;
    FunctionSignature signature;
    std::vector<std::string> param_names;
    std::vector<Instruction> instructions;
};

struct Module {
    std::vector<Global> globals;
    std::vector<Function> functions;
    std::map<std::string, FunctionSignature> signatures;
};

Module parseModule(const std::string &text);
std::string sanitizeSymbol(const std::string &name);

}  // namespace llvmir

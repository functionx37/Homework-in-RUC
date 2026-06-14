#include "llvm_ir.h"

#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

namespace llvmir {

Type::Type() : kind(TypeKind::Void), array_length(0), element() {}

Type Type::make(TypeKind valueKind) {
    Type type;
    type.kind = valueKind;
    return type;
}

Type Type::makePointer(const Type &elementType) {
    Type type;
    type.kind = TypeKind::Pointer;
    type.element.reset(new Type(elementType));
    return type;
}

Type Type::makeArray(int length, const Type &elementType) {
    Type type;
    type.kind = TypeKind::Array;
    type.array_length = length;
    type.element.reset(new Type(elementType));
    return type;
}

bool Type::operator==(const Type &other) const {
    if (kind != other.kind || array_length != other.array_length) {
        return false;
    }
    if (element.get() == NULL || other.element.get() == NULL) {
        return element.get() == other.element.get();
    }
    return *element == *other.element;
}

bool Type::operator!=(const Type &other) const {
    return !(*this == other);
}

size_t Type::size() const {
    switch (kind) {
        case TypeKind::I1:
        case TypeKind::I8:
            return 1;
        case TypeKind::I32:
        case TypeKind::Float:
            return 4;
        case TypeKind::I64:
        case TypeKind::Pointer:
            return 8;
        case TypeKind::Array:
            return static_cast<size_t>(array_length) * element->size();
        case TypeKind::Void:
            return 0;
    }
    return 0;
}

size_t Type::align() const {
    switch (kind) {
        case TypeKind::I1:
        case TypeKind::I8:
            return 1;
        case TypeKind::I32:
        case TypeKind::Float:
            return 4;
        case TypeKind::I64:
        case TypeKind::Pointer:
            return 8;
        case TypeKind::Array:
            return element->align();
        case TypeKind::Void:
            return 1;
    }
    return 1;
}

string Type::str() const {
    switch (kind) {
        case TypeKind::Void: return "void";
        case TypeKind::I1: return "i1";
        case TypeKind::I8: return "i8";
        case TypeKind::I32: return "i32";
        case TypeKind::I64: return "i64";
        case TypeKind::Float: return "float";
        case TypeKind::Pointer: return element->str() + "*";
        case TypeKind::Array: {
            ostringstream oss;
            oss << "[" << array_length << " x " << element->str() << "]";
            return oss.str();
        }
    }
    return "";
}

Value::Value() : kind(ValueKind::IntImmediate), name(), int_value(0), float_bits(0) {}

Instruction::Instruction()
    : kind(InstructionKind::Label), name(), type(), aux_type(), binary_op(BinaryOp::Add),
      cmp_op(CmpOp::Eq), cast_op(CastOp::Sitofp), lhs(), rhs(), value(), ptr(),
      gep_base_type(), indices(), args(), callee(), has_result(false), is_vararg(false),
      true_label(), false_label(), target_label() {}

Initializer::Initializer()
    : kind(Kind::Zero), int_value(0), float_bits(0), elements(), bytes() {}

FunctionSignature::FunctionSignature() : return_type(), param_types(), is_vararg(false) {}

static string trim(const string &text) {
    size_t start = 0;
    while (start < text.size() && isspace(static_cast<unsigned char>(text[start]))) {
        ++start;
    }
    size_t end = text.size();
    while (end > start && isspace(static_cast<unsigned char>(text[end - 1]))) {
        --end;
    }
    return text.substr(start, end - start);
}

static vector<string> splitLines(const string &text) {
    vector<string> lines;
    string current;
    for (size_t i = 0; i < text.size(); ++i) {
        char ch = text[i];
        if (ch == '\n') {
            lines.push_back(current);
            current.clear();
        } else if (ch != '\r') {
            current.push_back(ch);
        }
    }
    if (!current.empty()) {
        lines.push_back(current);
    }
    return lines;
}

static vector<string> splitTopLevel(const string &text, char delim) {
    vector<string> parts;
    string current;
    int squareDepth = 0;
    int parenDepth = 0;
    bool inString = false;
    for (size_t i = 0; i < text.size(); ++i) {
        char ch = text[i];
        if (inString) {
            current.push_back(ch);
            if (ch == '\\' && i + 1 < text.size()) {
                current.push_back(text[++i]);
                continue;
            }
            if (ch == '"') {
                inString = false;
            }
            continue;
        }
        if (ch == '"') {
            inString = true;
            current.push_back(ch);
            continue;
        }
        if (ch == '[') ++squareDepth;
        if (ch == ']') --squareDepth;
        if (ch == '(') ++parenDepth;
        if (ch == ')') --parenDepth;
        if (ch == delim && squareDepth == 0 && parenDepth == 0) {
            parts.push_back(trim(current));
            current.clear();
            continue;
        }
        current.push_back(ch);
    }
    if (!current.empty()) {
        parts.push_back(trim(current));
    }
    return parts;
}

static uint32_t parseFloatBits(const string &text) {
    if (text.size() > 2 && text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) {
        unsigned long long bits = strtoull(text.c_str(), NULL, 16);
        double value = 0.0;
        memcpy(&value, &bits, sizeof(bits));
        float asFloat = static_cast<float>(value);
        uint32_t out = 0;
        memcpy(&out, &asFloat, sizeof(out));
        return out;
    }
    float value = strtof(text.c_str(), NULL);
    uint32_t out = 0;
    memcpy(&out, &value, sizeof(out));
    return out;
}

class TypeParser {
public:
    explicit TypeParser(const string &text) : src(text), pos(0) {}

    Type parse() {
        skipSpaces();
        Type type = parsePrimary();
        skipSpaces();
        while (pos < src.size() && src[pos] == '*') {
            ++pos;
            type = Type::makePointer(type);
            skipSpaces();
        }
        return type;
    }

    size_t position() const {
        return pos;
    }

private:
    const string &src;
    size_t pos;

    void skipSpaces() {
        while (pos < src.size() && isspace(static_cast<unsigned char>(src[pos]))) {
            ++pos;
        }
    }

    Type parsePrimary() {
        if (pos >= src.size()) {
            throw runtime_error("Unexpected end while parsing type");
        }
        if (src[pos] == '[') {
            ++pos;
            skipSpaces();
            size_t start = pos;
            while (pos < src.size() && isdigit(static_cast<unsigned char>(src[pos]))) {
                ++pos;
            }
            int count = atoi(src.substr(start, pos - start).c_str());
            skipSpaces();
            if (pos >= src.size() || src[pos] != 'x') {
                throw runtime_error("Expected x in array type");
            }
            ++pos;
            Type elementType = parse();
            skipSpaces();
            if (pos >= src.size() || src[pos] != ']') {
                throw runtime_error("Expected ] in array type");
            }
            ++pos;
            return Type::makeArray(count, elementType);
        }
        if (src.compare(pos, 4, "void") == 0) {
            pos += 4;
            return Type::make(TypeKind::Void);
        }
        if (src.compare(pos, 2, "i1") == 0) {
            pos += 2;
            return Type::make(TypeKind::I1);
        }
        if (src.compare(pos, 2, "i8") == 0) {
            pos += 2;
            return Type::make(TypeKind::I8);
        }
        if (src.compare(pos, 3, "i32") == 0) {
            pos += 3;
            return Type::make(TypeKind::I32);
        }
        if (src.compare(pos, 3, "i64") == 0) {
            pos += 3;
            return Type::make(TypeKind::I64);
        }
        if (src.compare(pos, 5, "float") == 0) {
            pos += 5;
            return Type::make(TypeKind::Float);
        }
        throw runtime_error("Unsupported LLVM IR type near: " + src.substr(pos));
    }
};

static Type parseType(const string &text) {
    TypeParser parser(text);
    Type type = parser.parse();
    if (trim(text.substr(parser.position())).empty()) {
        return type;
    }
    throw runtime_error("Trailing characters after type: " + text);
}

static Value parseValueToken(const string &token, const Type &type) {
    string text = trim(token);
    Value value;
    if (text.empty()) {
        throw runtime_error("Empty value token");
    }
    if (text[0] == '%') {
        value.kind = ValueKind::Local;
        value.name = text.substr(1);
        return value;
    }
    if (text[0] == '@') {
        value.kind = ValueKind::Global;
        value.name = text.substr(1);
        return value;
    }
    if (text == "null") {
        value.kind = ValueKind::Null;
        return value;
    }
    if (type.kind == TypeKind::Float) {
        value.kind = ValueKind::FloatImmediate;
        value.float_bits = parseFloatBits(text);
        return value;
    }
    value.kind = ValueKind::IntImmediate;
    value.int_value = strtoll(text.c_str(), NULL, 10);
    return value;
}

static TypedValue parseTypedValue(const string &text) {
    TypeParser parser(text);
    TypedValue typed;
    typed.type = parser.parse();
    typed.value = parseValueToken(text.substr(parser.position()), typed.type);
    return typed;
}

class InitParser {
public:
    explicit InitParser(const string &text) : src(text), pos(0) {}

    Initializer parse(const Type &type) {
        skipSpaces();
        if (matchWord("zeroinitializer")) {
            Initializer init;
            init.kind = Initializer::Kind::Zero;
            return init;
        }
        if (type.kind == TypeKind::Array && type.element->kind == TypeKind::I8 && peek() == 'c') {
            return parseStringBytes();
        }
        if (type.kind == TypeKind::Array) {
            return parseArray(type);
        }
        if (type.kind == TypeKind::Float) {
            Initializer init;
            init.kind = Initializer::Kind::ScalarFloat;
            init.float_bits = parseFloatBits(readToken());
            return init;
        }
        if (type.kind == TypeKind::I1 || type.kind == TypeKind::I8 || type.kind == TypeKind::I32 ||
            type.kind == TypeKind::I64 || type.kind == TypeKind::Pointer) {
            Initializer init;
            init.kind = Initializer::Kind::ScalarInt;
            init.int_value = strtoll(readToken().c_str(), NULL, 10);
            return init;
        }
        throw runtime_error("Unsupported initializer type: " + type.str());
    }

private:
    const string &src;
    size_t pos;

    char peek() const {
        return pos < src.size() ? src[pos] : '\0';
    }

    void skipSpaces() {
        while (pos < src.size() && isspace(static_cast<unsigned char>(src[pos]))) {
            ++pos;
        }
    }

    bool matchWord(const string &word) {
        skipSpaces();
        if (src.compare(pos, word.size(), word) != 0) {
            return false;
        }
        pos += word.size();
        return true;
    }

    string readToken() {
        skipSpaces();
        size_t start = pos;
        while (pos < src.size() && !isspace(static_cast<unsigned char>(src[pos])) &&
               src[pos] != ',' && src[pos] != ']') {
            ++pos;
        }
        return src.substr(start, pos - start);
    }

    void expect(char ch) {
        skipSpaces();
        if (pos >= src.size() || src[pos] != ch) {
            throw runtime_error(string("Expected ") + ch + " in initializer");
        }
        ++pos;
    }

    Initializer parseStringBytes() {
        skipSpaces();
        if (peek() != 'c') {
            throw runtime_error("Expected c\"...\" string initializer");
        }
        ++pos;
        expect('"');
        Initializer init;
        init.kind = Initializer::Kind::StringBytes;
        while (pos < src.size()) {
            char ch = src[pos++];
            if (ch == '"') {
                return init;
            }
            if (ch == '\\') {
                if (pos + 1 >= src.size()) {
                    throw runtime_error("Invalid string escape");
                }
                int hi = isdigit(static_cast<unsigned char>(src[pos])) ? src[pos] - '0'
                                                                        : (tolower(src[pos]) - 'a' + 10);
                int lo = isdigit(static_cast<unsigned char>(src[pos + 1])) ? src[pos + 1] - '0'
                                                                            : (tolower(src[pos + 1]) - 'a' + 10);
                init.bytes.push_back(static_cast<unsigned char>((hi << 4) | lo));
                pos += 2;
                continue;
            }
            init.bytes.push_back(static_cast<unsigned char>(ch));
        }
        throw runtime_error("Unterminated string initializer");
    }

    Initializer parseArray(const Type &type) {
        expect('[');
        Initializer init;
        init.kind = Initializer::Kind::Array;
        while (true) {
            skipSpaces();
            if (peek() == ']') {
                ++pos;
                break;
            }
            string remaining = src.substr(pos);
            TypeParser typeParser(remaining);
            Type elementType = typeParser.parse();
            if (elementType != *type.element) {
                throw runtime_error("Array initializer type mismatch");
            }
            pos += typeParser.position();
            init.elements.push_back(parse(elementType));
            skipSpaces();
            if (peek() == ',') {
                ++pos;
                continue;
            }
            if (peek() == ']') {
                ++pos;
                break;
            }
            throw runtime_error("Malformed array initializer");
        }
        return init;
    }
};

static Initializer parseInitializer(const Type &type, const string &text) {
    InitParser parser(text);
    return parser.parse(type);
}

static FunctionSignature parseSignatureHeader(const string &text, string *nameOut,
                                              vector<string> *paramNamesOut, bool isDefinition) {
    size_t atPos = text.find('@');
    if (atPos == string::npos) {
        throw runtime_error("Malformed function signature: " + text);
    }
    string retPart = trim(text.substr(0, atPos));
    TypeParser retParser(retPart);
    FunctionSignature sig;
    sig.return_type = retParser.parse();
    size_t nameEnd = text.find('(', atPos);
    if (nameEnd == string::npos) {
        throw runtime_error("Malformed function signature: " + text);
    }
    *nameOut = text.substr(atPos + 1, nameEnd - atPos - 1);
    size_t close = text.rfind(')');
    string params = close == string::npos ? "" : text.substr(nameEnd + 1, close - nameEnd - 1);
    vector<string> parts = splitTopLevel(params, ',');
    for (size_t i = 0; i < parts.size(); ++i) {
        if (parts[i].empty()) {
            continue;
        }
        if (parts[i] == "...") {
            sig.is_vararg = true;
            continue;
        }
        if (isDefinition) {
            size_t split = parts[i].rfind(' ');
            if (split == string::npos) {
                throw runtime_error("Malformed function parameter: " + parts[i]);
            }
            sig.param_types.push_back(parseType(parts[i].substr(0, split)));
            paramNamesOut->push_back(trim(parts[i].substr(split + 1)).substr(1));
        } else {
            sig.param_types.push_back(parseType(parts[i]));
        }
    }
    return sig;
}

static BinaryOp parseBinaryOp(const string &name) {
    if (name == "add") return BinaryOp::Add;
    if (name == "sub") return BinaryOp::Sub;
    if (name == "mul") return BinaryOp::Mul;
    if (name == "sdiv") return BinaryOp::SDiv;
    if (name == "srem") return BinaryOp::SRem;
    if (name == "fadd") return BinaryOp::FAdd;
    if (name == "fsub") return BinaryOp::FSub;
    if (name == "fmul") return BinaryOp::FMul;
    if (name == "fdiv") return BinaryOp::FDiv;
    throw runtime_error("Unsupported binary op: " + name);
}

static CmpOp parseCmpOp(const string &name) {
    if (name == "eq") return CmpOp::Eq;
    if (name == "ne") return CmpOp::Ne;
    if (name == "slt") return CmpOp::Slt;
    if (name == "sgt") return CmpOp::Sgt;
    if (name == "sle") return CmpOp::Sle;
    if (name == "sge") return CmpOp::Sge;
    if (name == "oeq") return CmpOp::OEq;
    if (name == "one") return CmpOp::ONe;
    if (name == "olt") return CmpOp::OLt;
    if (name == "ogt") return CmpOp::OGt;
    if (name == "ole") return CmpOp::OLe;
    if (name == "oge") return CmpOp::OGe;
    throw runtime_error("Unsupported compare op: " + name);
}

static Instruction parseCallInstruction(const string &text, const string &resultName) {
    Instruction inst;
    inst.kind = InstructionKind::Call;
    inst.has_result = !resultName.empty();
    inst.name = resultName;

    string rest = trim(text.substr(5));
    size_t atPos = rest.find('@');
    size_t openPos = rest.find('(', atPos);
    size_t closePos = rest.rfind(')');
    if (atPos == string::npos || openPos == string::npos || closePos == string::npos) {
        throw runtime_error("Malformed call instruction: " + text);
    }
    string retSpec = trim(rest.substr(0, atPos));
    TypeParser retParser(retSpec);
    inst.type = retParser.parse();
    inst.is_vararg = trim(retSpec.substr(retParser.position())).find('(') != string::npos;
    inst.callee = rest.substr(atPos + 1, openPos - atPos - 1);
    vector<string> args = splitTopLevel(rest.substr(openPos + 1, closePos - openPos - 1), ',');
    for (size_t i = 0; i < args.size(); ++i) {
        if (!args[i].empty()) {
            inst.args.push_back(parseTypedValue(args[i]));
        }
    }
    return inst;
}

static Instruction parseInstruction(const string &line) {
    string text = trim(line);
    if (text.empty()) {
        return Instruction();
    }
    if (!text.empty() && text[text.size() - 1] == ':') {
        Instruction inst;
        inst.kind = InstructionKind::Label;
        inst.name = text.substr(0, text.size() - 1);
        return inst;
    }
    if (text.compare(0, 6, "store ") == 0) {
        Instruction inst;
        inst.kind = InstructionKind::Store;
        vector<string> parts = splitTopLevel(text.substr(6), ',');
        if (parts.size() != 2) {
            throw runtime_error("Malformed store instruction: " + text);
        }
        inst.value = parseTypedValue(parts[0]);
        inst.ptr = parseTypedValue(parts[1]);
        return inst;
    }
    if (text.compare(0, 3, "br ") == 0) {
        Instruction inst;
        if (text.compare(3, 5, "label") == 0) {
            inst.kind = InstructionKind::BrUncond;
            string target = trim(text.substr(8));
            inst.target_label = target.substr(1);
            return inst;
        }
        inst.kind = InstructionKind::BrCond;
        vector<string> parts = splitTopLevel(text.substr(3), ',');
        if (parts.size() != 3) {
            throw runtime_error("Malformed branch instruction: " + text);
        }
        inst.value = parseTypedValue(parts[0]);
        string truePart = trim(parts[1]);
        string falsePart = trim(parts[2]);
        if (truePart.compare(0, 6, "label ") != 0 || falsePart.compare(0, 6, "label ") != 0) {
            throw runtime_error("Malformed conditional branch labels: " + text);
        }
        inst.true_label = trim(truePart.substr(6));
        inst.false_label = trim(falsePart.substr(6));
        if (!inst.true_label.empty() && inst.true_label[0] == '%') inst.true_label = inst.true_label.substr(1);
        if (!inst.false_label.empty() && inst.false_label[0] == '%') inst.false_label = inst.false_label.substr(1);
        return inst;
    }
    if (text.compare(0, 4, "ret ") == 0) {
        Instruction inst;
        inst.kind = InstructionKind::Ret;
        if (text == "ret void") {
            inst.type = Type::make(TypeKind::Void);
        } else {
            inst.value = parseTypedValue(text.substr(4));
            inst.type = inst.value.type;
        }
        return inst;
    }
    if (text.compare(0, 5, "call ") == 0) {
        return parseCallInstruction(text, "");
    }

    size_t eqPos = text.find(" = ");
    if (eqPos == string::npos) {
        throw runtime_error("Unsupported LLVM IR instruction: " + text);
    }
    string name = text.substr(1, eqPos - 1);
    string rhs = text.substr(eqPos + 3);

    if (rhs.compare(0, 7, "alloca ") == 0) {
        Instruction inst;
        inst.kind = InstructionKind::Alloca;
        inst.name = name;
        inst.type = Type::makePointer(parseType(rhs.substr(7)));
        inst.aux_type = *inst.type.element;
        inst.has_result = true;
        return inst;
    }
    if (rhs.compare(0, 5, "load ") == 0) {
        Instruction inst;
        inst.kind = InstructionKind::Load;
        inst.name = name;
        vector<string> parts = splitTopLevel(rhs.substr(5), ',');
        inst.type = parseType(parts[0]);
        inst.ptr = parseTypedValue(parts[1]);
        inst.has_result = true;
        return inst;
    }
    if (rhs.compare(0, 5, "icmp ") == 0 || rhs.compare(0, 5, "fcmp ") == 0) {
        Instruction inst;
        inst.kind = InstructionKind::Cmp;
        inst.name = name;
        bool isFloatCmp = rhs[0] == 'f';
        size_t predEnd = rhs.find(' ', 5);
        inst.cmp_op = parseCmpOp(rhs.substr(5, predEnd - 5));
        vector<string> parts = splitTopLevel(rhs.substr(predEnd + 1), ',');
        inst.lhs = parseTypedValue(parts[0]);
        inst.rhs.value = parseValueToken(parts[1], inst.lhs.type);
        inst.rhs.type = inst.lhs.type;
        inst.type = Type::make(TypeKind::I1);
        inst.has_result = true;
        (void)isFloatCmp;
        return inst;
    }
    if (rhs.compare(0, 5, "zext ") == 0) {
        Instruction inst;
        inst.kind = InstructionKind::ZExt;
        inst.name = name;
        size_t toPos = rhs.rfind(" to ");
        inst.value = parseTypedValue(rhs.substr(5, toPos - 5));
        inst.type = parseType(rhs.substr(toPos + 4));
        inst.has_result = true;
        return inst;
    }
    if (rhs.compare(0, 4, "xor ") == 0) {
        Instruction inst;
        inst.kind = InstructionKind::Xor;
        inst.name = name;
        vector<string> parts = splitTopLevel(rhs.substr(4), ',');
        inst.lhs = parseTypedValue(parts[0]);
        inst.rhs.type = inst.lhs.type;
        inst.rhs.value = parseValueToken(parts[1], inst.lhs.type);
        inst.type = inst.lhs.type;
        inst.has_result = true;
        return inst;
    }
    if (rhs.compare(0, 5, "call ") == 0) {
        Instruction inst = parseCallInstruction(rhs, name);
        inst.has_result = true;
        return inst;
    }
    if (rhs.compare(0, 8, "bitcast ") == 0) {
        Instruction inst;
        inst.kind = InstructionKind::Bitcast;
        inst.name = name;
        size_t toPos = rhs.rfind(" to ");
        inst.value = parseTypedValue(rhs.substr(8, toPos - 8));
        inst.type = parseType(rhs.substr(toPos + 4));
        inst.has_result = true;
        return inst;
    }
    if (rhs.compare(0, 14, "getelementptr ") == 0) {
        Instruction inst;
        inst.kind = InstructionKind::GetElementPtr;
        inst.name = name;
        string rest = rhs.substr(14);
        if (rest.compare(0, 9, "inbounds ") == 0) {
            rest = rest.substr(9);
        }
        vector<string> parts = splitTopLevel(rest, ',');
        inst.gep_base_type = parseType(parts[0]);
        inst.ptr = parseTypedValue(parts[1]);
        for (size_t i = 2; i < parts.size(); ++i) {
            inst.indices.push_back(parseTypedValue(parts[i]));
        }
        inst.type = Type::makePointer(inst.gep_base_type);
        inst.has_result = true;
        return inst;
    }
    if (rhs.compare(0, 7, "sitofp ") == 0 || rhs.compare(0, 7, "fptosi ") == 0) {
        Instruction inst;
        inst.kind = InstructionKind::Cast;
        inst.name = name;
        inst.cast_op = rhs[0] == 's' ? CastOp::Sitofp : CastOp::Fptosi;
        size_t toPos = rhs.rfind(" to ");
        inst.value = parseTypedValue(rhs.substr(7, toPos - 7));
        inst.type = parseType(rhs.substr(toPos + 4));
        inst.has_result = true;
        return inst;
    }

    vector<string> words = splitTopLevel(rhs, ' ');
    if (!words.empty()) {
        const string &op = words[0];
        if (op == "add" || op == "sub" || op == "mul" || op == "sdiv" || op == "srem" ||
            op == "fadd" || op == "fsub" || op == "fmul" || op == "fdiv") {
            Instruction inst;
            inst.kind = InstructionKind::Binary;
            inst.name = name;
            inst.binary_op = parseBinaryOp(op);
            size_t firstSpace = rhs.find(' ');
            vector<string> parts = splitTopLevel(rhs.substr(firstSpace + 1), ',');
            inst.lhs = parseTypedValue(parts[0]);
            inst.rhs.type = inst.lhs.type;
            inst.rhs.value = parseValueToken(parts[1], inst.lhs.type);
            inst.type = inst.lhs.type;
            inst.has_result = true;
            return inst;
        }
    }
    throw runtime_error("Unsupported LLVM IR instruction: " + rhs);
}

Module parseModule(const string &text) {
    Module module;
    vector<string> lines = splitLines(text);
    bool inFunction = false;
    Function current;
    for (size_t i = 0; i < lines.size(); ++i) {
        string line = trim(lines[i]);
        if (line.empty()) {
            continue;
        }
        if (!inFunction) {
            if (line.compare(0, 8, "declare ") == 0) {
                string name;
                vector<string> params;
                FunctionSignature sig = parseSignatureHeader(line.substr(8), &name, &params, false);
                module.signatures[name] = sig;
                continue;
            }
            if (line.compare(0, 7, "define ") == 0) {
                inFunction = true;
                current = Function();
                current.signature = parseSignatureHeader(line.substr(7), &current.name, &current.param_names, true);
                module.signatures[current.name] = current.signature;
                continue;
            }
            size_t eqPos = line.find('=');
            if (eqPos == string::npos) {
                throw runtime_error("Malformed global definition: " + line);
            }
            Global global;
            global.name = trim(line.substr(1, eqPos - 2));
            string rest = trim(line.substr(eqPos + 1));
            global.is_private = false;
            while (true) {
                if (rest.compare(0, 8, "private ") == 0) {
                    global.is_private = true;
                    rest = trim(rest.substr(8));
                    continue;
                }
                if (rest.compare(0, 9, "internal ") == 0) {
                    global.is_private = true;
                    rest = trim(rest.substr(9));
                    continue;
                }
                if (rest.compare(0, 13, "unnamed_addr ") == 0) {
                    rest = trim(rest.substr(13));
                    continue;
                }
                break;
            }
            if (rest.compare(0, 9, "constant ") == 0) {
                global.is_constant = true;
                rest = trim(rest.substr(9));
            } else if (rest.compare(0, 7, "global ") == 0) {
                global.is_constant = false;
                rest = trim(rest.substr(7));
            } else {
                throw runtime_error("Unsupported global linkage: " + line);
            }
            TypeParser typeParser(rest);
            global.type = typeParser.parse();
            global.init = parseInitializer(global.type, rest.substr(typeParser.position()));
            module.globals.push_back(global);
            continue;
        }

        if (line == "}") {
            module.functions.push_back(current);
            inFunction = false;
            continue;
        }
        current.instructions.push_back(parseInstruction(line));
    }
    return module;
}

string sanitizeSymbol(const string &name) {
    string in = name;
    if (!in.empty() && (in[0] == '@' || in[0] == '%')) {
        in = in.substr(1);
    }
    string out;
    for (size_t i = 0; i < in.size(); ++i) {
        char ch = in[i];
        if (isalnum(static_cast<unsigned char>(ch)) || ch == '_' || ch == '.' || ch == '$') {
            out.push_back(ch);
        } else {
            out.push_back('_');
        }
    }
    if (out.empty()) {
        return "_";
    }
    return out;
}

}  // namespace llvmir

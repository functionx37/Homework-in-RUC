#include "ir_codegen.h"

#include "utils.h"

#include <cstring>
#include <iomanip>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

using namespace std;

struct ConstValue {
    BaseType baseType;
    bool isArray;
    double scalar;
    vector<int> dims;
    vector<double> values;
};

struct ParamSig {
    BaseType baseType;
    bool isArray;
    vector<int> dims;
};

struct FuncSig {
    bool isVoid;
    BaseType returnType;
    vector<ParamSig> params;
};

struct Symbol {
    BaseType baseType;
    bool isConst;
    bool isGlobal;
    bool isArrayParam;
    string addr;
    vector<int> dims;
    int totalRank;
    string paramPointeeType;
    bool hasConstScalar;
    double constScalar;
    vector<double> constArrayValues;
};

struct AddrInfo {
    string ptr;
    bool isScalar;
    BaseType baseType;
    string pointeeType;
};

struct TypedValue {
    BaseType type;
    string ir;
};

struct ConstEvalValue {
    BaseType type;
    double value;
};

class CodeGen {
public:
    explicit CodeGen(const CompUnit &program) : prog(program), tempId(0), labelId(0) {
        pushScope();
        collectBuiltinFunctionSignatures();
        primeGlobalConstScope();
        collectFunctionSignatures();
        collectPrintfStrings();
        collectBuiltinUsages();
        ensureRuntimeStrings();
    }

    string generate() {
        vector<string> lines;
        lines.push_back("declare i32 @printf(i8*, ...)");
        lines.push_back("declare i8* @memcpy(i8*, i8*, i64)");
        lines.push_back("declare i8* @memset(i8*, i32, i64)");
        emitRuntimeBuiltinDecls(lines);
        lines.push_back("");
        lines.push_back(internalHasOutputName() + " = internal global i32 0");
        lines.push_back(internalLastNewlineName() + " = internal global i32 0");
        lines.push_back("");
        emitStringGlobals(lines);
        emitGlobalDecls(lines);
        emitFunctions(lines);
        emitConstArrayTemplateGlobals(lines);
        emitMainWrapper(lines);
        return join(lines, "\n") + "\n";
    }

private:
    const CompUnit &prog;
    map<string, FuncSig> funcSigs;
    map<string, pair<string, string> > stringPool;
    map<string, string> constArrayTemplatePool;
    vector<string> constArrayTemplateDefs;
    map<string, bool> userFunctionNames;
    map<string, bool> usedRuntimeBuiltins;
    vector<map<string, Symbol> > symbolScopes;
    vector<map<string, ConstValue> > constScopes;
    vector<string> allocas;
    vector<string> code;
    vector<pair<string, string> > loopLabels;
    string currentReturnLabel;
    string currentRetSlot;
    bool currentFuncIsVoid;
    BaseType currentFuncReturnType;
    int tempId;
    int labelId;

    void collectBuiltinFunctionSignatures() {
        FuncSig getIntSig;
        getIntSig.isVoid = false;
        getIntSig.returnType = BaseType::Int;
        funcSigs["getint"] = getIntSig;

        FuncSig getChSig;
        getChSig.isVoid = false;
        getChSig.returnType = BaseType::Int;
        funcSigs["getch"] = getChSig;

        FuncSig getFloatSig;
        getFloatSig.isVoid = false;
        getFloatSig.returnType = BaseType::Float;
        funcSigs["getfloat"] = getFloatSig;

        FuncSig getArraySig;
        getArraySig.isVoid = false;
        getArraySig.returnType = BaseType::Int;
        ParamSig getArrayParam;
        getArrayParam.baseType = BaseType::Int;
        getArrayParam.isArray = true;
        getArraySig.params.push_back(getArrayParam);
        funcSigs["getarray"] = getArraySig;

        FuncSig getFArraySig;
        getFArraySig.isVoid = false;
        getFArraySig.returnType = BaseType::Int;
        ParamSig getFArrayParam;
        getFArrayParam.baseType = BaseType::Float;
        getFArrayParam.isArray = true;
        getFArraySig.params.push_back(getFArrayParam);
        funcSigs["getfarray"] = getFArraySig;

        FuncSig putIntSig;
        putIntSig.isVoid = true;
        ParamSig putIntParam;
        putIntParam.baseType = BaseType::Int;
        putIntParam.isArray = false;
        putIntSig.params.push_back(putIntParam);
        funcSigs["putint"] = putIntSig;

        FuncSig putChSig;
        putChSig.isVoid = true;
        ParamSig putChParam;
        putChParam.baseType = BaseType::Int;
        putChParam.isArray = false;
        putChSig.params.push_back(putChParam);
        funcSigs["putch"] = putChSig;

        FuncSig putFloatSig;
        putFloatSig.isVoid = true;
        ParamSig putFloatParam;
        putFloatParam.baseType = BaseType::Float;
        putFloatParam.isArray = false;
        putFloatSig.params.push_back(putFloatParam);
        funcSigs["putfloat"] = putFloatSig;

        FuncSig putArraySig;
        putArraySig.isVoid = true;
        ParamSig putArrayParam0;
        putArrayParam0.baseType = BaseType::Int;
        putArrayParam0.isArray = false;
        putArraySig.params.push_back(putArrayParam0);
        ParamSig putArrayParam1;
        putArrayParam1.baseType = BaseType::Int;
        putArrayParam1.isArray = true;
        putArraySig.params.push_back(putArrayParam1);
        funcSigs["putarray"] = putArraySig;

        FuncSig putFArraySig;
        putFArraySig.isVoid = true;
        ParamSig putFArrayParam0;
        putFArrayParam0.baseType = BaseType::Int;
        putFArrayParam0.isArray = false;
        putFArraySig.params.push_back(putFArrayParam0);
        ParamSig putFArrayParam1;
        putFArrayParam1.baseType = BaseType::Float;
        putFArrayParam1.isArray = true;
        putFArraySig.params.push_back(putFArrayParam1);
        funcSigs["putfarray"] = putFArraySig;

        FuncSig startTimeSig;
        startTimeSig.isVoid = true;
        funcSigs["starttime"] = startTimeSig;

        FuncSig stopTimeSig;
        stopTimeSig.isVoid = true;
        funcSigs["stoptime"] = stopTimeSig;

        FuncSig rawStartTimeSig;
        rawStartTimeSig.isVoid = true;
        ParamSig rawStartTimeParam;
        rawStartTimeParam.baseType = BaseType::Int;
        rawStartTimeParam.isArray = false;
        rawStartTimeSig.params.push_back(rawStartTimeParam);
        funcSigs["_sysy_starttime"] = rawStartTimeSig;

        FuncSig rawStopTimeSig;
        rawStopTimeSig.isVoid = true;
        ParamSig rawStopTimeParam;
        rawStopTimeParam.baseType = BaseType::Int;
        rawStopTimeParam.isArray = false;
        rawStopTimeSig.params.push_back(rawStopTimeParam);
        funcSigs["_sysy_stoptime"] = rawStopTimeSig;
    }

    void primeGlobalConstScope() {
        for (size_t i = 0; i < prog.globals.size(); ++i) {
            if (!prog.globals[i].isConst) {
                continue;
            }
            for (size_t j = 0; j < prog.globals[i].defs.size(); ++j) {
                const VarDef &def = prog.globals[i].defs[j];
                vector<int> dims = evalDims(def.dims);
                ConstValue cv;
                cv.baseType = prog.globals[i].baseType;
                cv.isArray = !dims.empty();
                if (dims.empty()) {
                    cv.scalar = evalConstExpr(def.init->expr).value;
                } else {
                    cv.dims = dims;
                    cv.values = buildConstArrayValues(dims, def.init);
                }
                addConstBinding(def.name, cv);
            }
        }
    }

    void collectFunctionSignatures() {
        for (size_t i = 0; i < prog.funcs.size(); ++i) {
            userFunctionNames[prog.funcs[i].name] = true;
            FuncSig sig;
            sig.isVoid = prog.funcs[i].isVoid;
            sig.returnType = prog.funcs[i].returnType;
            for (size_t j = 0; j < prog.funcs[i].params.size(); ++j) {
                ParamSig param;
                param.baseType = prog.funcs[i].params[j].baseType;
                param.isArray = prog.funcs[i].params[j].isArray;
                param.dims = evalDims(prog.funcs[i].params[j].dims);
                sig.params.push_back(param);
            }
            funcSigs[prog.funcs[i].name] = sig;
        }
    }

    void collectPrintfStrings() {
        for (size_t i = 0; i < prog.funcs.size(); ++i) {
            collectPrintfStringsInBlock(prog.funcs[i].block);
        }
    }

    bool hasUserFunction(const string &name) const {
        return userFunctionNames.find(name) != userFunctionNames.end();
    }

    static bool isRuntimeBuiltinName(const string &name) {
        return name == "getint" || name == "getch" || name == "getfloat" ||
               name == "getarray" || name == "getfarray" ||
               name == "putint" || name == "putch" || name == "putfloat" ||
               name == "putarray" || name == "putfarray" ||
               name == "starttime" || name == "stoptime" ||
               name == "_sysy_starttime" || name == "_sysy_stoptime";
    }

    bool isRuntimeBuiltinCall(const string &name) const {
        return isRuntimeBuiltinName(name) && !hasUserFunction(name);
    }

    void markRuntimeBuiltinUsed(const string &name) {
        if (isRuntimeBuiltinCall(name)) {
            usedRuntimeBuiltins[name] = true;
        }
    }

    void collectBuiltinUsages() {
        for (size_t i = 0; i < prog.funcs.size(); ++i) {
            collectBuiltinUsagesInBlock(prog.funcs[i].block);
        }
    }

    void collectBuiltinUsagesInBlock(const shared_ptr<BlockStmt> &block) {
        for (size_t i = 0; i < block->items.size(); ++i) {
            if (block->items[i].isDecl) {
                collectBuiltinUsagesInDecl(block->items[i].decl);
            } else {
                collectBuiltinUsagesInStmt(block->items[i].stmt);
            }
        }
    }

    void collectBuiltinUsagesInDecl(const Decl &decl) {
        for (size_t i = 0; i < decl.defs.size(); ++i) {
            if (decl.defs[i].init) {
                collectBuiltinUsagesInInit(decl.defs[i].init);
            }
        }
    }

    void collectBuiltinUsagesInInit(const shared_ptr<InitVal> &init) {
        if (!init) {
            return;
        }
        if (init->isExpr) {
            collectBuiltinUsagesInExpr(init->expr);
            return;
        }
        for (size_t i = 0; i < init->list.size(); ++i) {
            collectBuiltinUsagesInInit(init->list[i]);
        }
    }

    void collectBuiltinUsagesInExpr(const shared_ptr<Expr> &expr) {
        if (!expr) {
            return;
        }
        if (dynamic_cast<LValExpr *>(expr.get()) != NULL) {
            const LVal &lval = static_pointer_cast<LValExpr>(expr)->lval;
            for (size_t i = 0; i < lval.indices.size(); ++i) {
                collectBuiltinUsagesInExpr(lval.indices[i]);
            }
            return;
        }
        if (dynamic_cast<UnaryExpr *>(expr.get()) != NULL) {
            collectBuiltinUsagesInExpr(static_pointer_cast<UnaryExpr>(expr)->operand);
            return;
        }
        if (dynamic_cast<BinaryExpr *>(expr.get()) != NULL) {
            shared_ptr<BinaryExpr> b = static_pointer_cast<BinaryExpr>(expr);
            collectBuiltinUsagesInExpr(b->lhs);
            collectBuiltinUsagesInExpr(b->rhs);
            return;
        }
        if (dynamic_cast<CallExpr *>(expr.get()) != NULL) {
            shared_ptr<CallExpr> c = static_pointer_cast<CallExpr>(expr);
            markRuntimeBuiltinUsed(c->callee);
            for (size_t i = 0; i < c->args.size(); ++i) {
                collectBuiltinUsagesInExpr(c->args[i]);
            }
        }
    }

    void collectBuiltinUsagesInStmt(const shared_ptr<Stmt> &stmt) {
        if (!stmt) {
            return;
        }
        if (dynamic_cast<BlockStmt *>(stmt.get()) != NULL) {
            collectBuiltinUsagesInBlock(static_pointer_cast<BlockStmt>(stmt));
            return;
        }
        if (dynamic_cast<AssignStmt *>(stmt.get()) != NULL) {
            shared_ptr<AssignStmt> s = static_pointer_cast<AssignStmt>(stmt);
            for (size_t i = 0; i < s->lval.indices.size(); ++i) {
                collectBuiltinUsagesInExpr(s->lval.indices[i]);
            }
            collectBuiltinUsagesInExpr(s->expr);
            return;
        }
        if (dynamic_cast<ExprStmt *>(stmt.get()) != NULL) {
            collectBuiltinUsagesInExpr(static_pointer_cast<ExprStmt>(stmt)->expr);
            return;
        }
        if (dynamic_cast<IfStmt *>(stmt.get()) != NULL) {
            shared_ptr<IfStmt> s = static_pointer_cast<IfStmt>(stmt);
            collectBuiltinUsagesInExpr(s->cond);
            collectBuiltinUsagesInStmt(s->thenStmt);
            collectBuiltinUsagesInStmt(s->elseStmt);
            return;
        }
        if (dynamic_cast<WhileStmt *>(stmt.get()) != NULL) {
            shared_ptr<WhileStmt> s = static_pointer_cast<WhileStmt>(stmt);
            collectBuiltinUsagesInExpr(s->cond);
            collectBuiltinUsagesInStmt(s->body);
            return;
        }
        if (dynamic_cast<ReturnStmt *>(stmt.get()) != NULL) {
            collectBuiltinUsagesInExpr(static_pointer_cast<ReturnStmt>(stmt)->expr);
            return;
        }
        if (dynamic_cast<GetIntStmt *>(stmt.get()) != NULL) {
            markRuntimeBuiltinUsed("getint");
            shared_ptr<GetIntStmt> s = static_pointer_cast<GetIntStmt>(stmt);
            for (size_t i = 0; i < s->lval.indices.size(); ++i) {
                collectBuiltinUsagesInExpr(s->lval.indices[i]);
            }
            return;
        }
        if (dynamic_cast<PrintfStmt *>(stmt.get()) != NULL) {
            shared_ptr<PrintfStmt> s = static_pointer_cast<PrintfStmt>(stmt);
            for (size_t i = 0; i < s->args.size(); ++i) {
                collectBuiltinUsagesInExpr(s->args[i]);
            }
        }
    }

    void emitRuntimeBuiltinDecls(vector<string> &lines) const {
        if (usedRuntimeBuiltins.find("getint") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare i32 @getint()");
        }
        if (usedRuntimeBuiltins.find("getch") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare i32 @getch()");
        }
        if (usedRuntimeBuiltins.find("getfloat") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare float @getfloat()");
        }
        if (usedRuntimeBuiltins.find("getarray") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare i32 @getarray(i32*)");
        }
        if (usedRuntimeBuiltins.find("getfarray") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare i32 @getfarray(float*)");
        }
        if (usedRuntimeBuiltins.find("putint") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare void @putint(i32)");
        }
        if (usedRuntimeBuiltins.find("putch") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare void @putch(i32)");
        }
        if (usedRuntimeBuiltins.find("putfloat") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare void @putfloat(float)");
        }
        if (usedRuntimeBuiltins.find("putarray") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare void @putarray(i32, i32*)");
        }
        if (usedRuntimeBuiltins.find("putfarray") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare void @putfarray(i32, float*)");
        }
        if (usedRuntimeBuiltins.find("starttime") != usedRuntimeBuiltins.end() ||
            usedRuntimeBuiltins.find("_sysy_starttime") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare void @_sysy_starttime(i32)");
        }
        if (usedRuntimeBuiltins.find("stoptime") != usedRuntimeBuiltins.end() ||
            usedRuntimeBuiltins.find("_sysy_stoptime") != usedRuntimeBuiltins.end()) {
            lines.push_back("declare void @_sysy_stoptime(i32)");
        }
    }

    void collectPrintfStringsInStmt(const shared_ptr<Stmt> &stmt) {
        if (!stmt) {
            return;
        }
        if (dynamic_cast<BlockStmt *>(stmt.get()) != NULL) {
            collectPrintfStringsInBlock(static_pointer_cast<BlockStmt>(stmt));
            return;
        }
        if (dynamic_cast<IfStmt *>(stmt.get()) != NULL) {
            shared_ptr<IfStmt> ifs = static_pointer_cast<IfStmt>(stmt);
            collectPrintfStringsInStmt(ifs->thenStmt);
            collectPrintfStringsInStmt(ifs->elseStmt);
            return;
        }
        if (dynamic_cast<WhileStmt *>(stmt.get()) != NULL) {
            collectPrintfStringsInStmt(static_pointer_cast<WhileStmt>(stmt)->body);
            return;
        }
        if (dynamic_cast<PrintfStmt *>(stmt.get()) != NULL) {
            shared_ptr<PrintfStmt> p = static_pointer_cast<PrintfStmt>(stmt);
            getStringName(stripQuotes(p->format));
        }
    }

    void collectPrintfStringsInBlock(const shared_ptr<BlockStmt> &block) {
        for (size_t i = 0; i < block->items.size(); ++i) {
            if (!block->items[i].isDecl) {
                collectPrintfStringsInStmt(block->items[i].stmt);
            }
        }
    }

    void ensureRuntimeStrings() {
        getStringName("%d\\n");
        getStringName("\\n");
    }

    string newTemp() {
        ostringstream oss;
        oss << "%t" << tempId++;
        return oss.str();
    }

    string newLabel(const string &prefix) {
        ostringstream oss;
        oss << prefix << labelId++;
        return oss.str();
    }

    void emit(const string &line) {
        code.push_back("  " + line);
    }

    void emitLabel(const string &label) {
        code.push_back(label + ":");
    }

    void emitAlloca(const string &line) {
        allocas.push_back("  " + line);
    }

    void pushScope() {
        symbolScopes.push_back(map<string, Symbol>());
        constScopes.push_back(map<string, ConstValue>());
    }

    void popScope() {
        symbolScopes.pop_back();
        constScopes.pop_back();
    }

    Symbol *findSymbol(const string &name) {
        for (int i = static_cast<int>(symbolScopes.size()) - 1; i >= 0; --i) {
            map<string, Symbol>::iterator it = symbolScopes[i].find(name);
            if (it != symbolScopes[i].end()) {
                return &it->second;
            }
        }
        return NULL;
    }

    const ConstValue *findConst(const string &name) const {
        for (int i = static_cast<int>(constScopes.size()) - 1; i >= 0; --i) {
            map<string, ConstValue>::const_iterator it = constScopes[i].find(name);
            if (it != constScopes[i].end()) {
                return &it->second;
            }
        }
        return NULL;
    }

    static bool isFloatType(BaseType type) {
        return type == BaseType::Float;
    }

    static string llvmType(BaseType type) {
        if (type == BaseType::Int) {
            return "i32";
        }
        if (type == BaseType::Float) {
            return "float";
        }
        return "void";
    }

    static string intValue(int v) {
        ostringstream oss;
        oss << v;
        return oss.str();
    }

    static string floatValue(double v) {
        float fv = static_cast<float>(v);
        if (fv == 0.0f) {
            return "0.000000e+00";
        }
        double dv = static_cast<double>(fv);
        uint64_t bits = 0;
        memcpy(&bits, &dv, sizeof(bits));
        ostringstream oss;
        oss << "0x" << uppercase << hex << setw(16) << setfill('0') << bits
            << nouppercase << dec << setfill(' ');
        return oss.str();
    }

    static string scalarLiteral(BaseType type, double value) {
        return type == BaseType::Float ? floatValue(value)
                                       : intValue(static_cast<int>(value));
    }

    static string arrayType(BaseType baseType, const vector<int> &dims, size_t idx = 0) {
        if (idx >= dims.size()) {
            return llvmType(baseType);
        }
        ostringstream oss;
        oss << "[" << dims[idx] << " x " << arrayType(baseType, dims, idx + 1) << "]";
        return oss.str();
    }

    static string pointerType(const string &baseType) {
        return baseType + "*";
    }

    static string arrayParamPointeeType(BaseType baseType, const vector<int> &dims) {
        return dims.empty() ? llvmType(baseType) : arrayType(baseType, dims);
    }

    static string nestedArrayOrScalarType(BaseType baseType, const vector<int> &dims, size_t start) {
        return start >= dims.size() ? llvmType(baseType) : arrayType(baseType, dims, start);
    }

    static string internalUserMainName() {
        return "__sysy_user_main";
    }

    static string userFunctionIRName(const string &name) {
        return name == "main" ? internalUserMainName() : "__sysy_user_fn_" + name;
    }

    static string globalIRName(const string &name) {
        return "@__sysy_user_g_" + name;
    }

    static string internalHasOutputName() {
        return "@__sysy_internal_has_output";
    }

    static string internalLastNewlineName() {
        return "@__sysy_internal_last_is_newline";
    }

    string functionIRName(const string &name) const {
        if (name == "starttime" && !hasUserFunction(name)) {
            return "_sysy_starttime";
        }
        if (name == "stoptime" && !hasUserFunction(name)) {
            return "_sysy_stoptime";
        }
        if (hasUserFunction(name)) {
            return userFunctionIRName(name);
        }
        return name;
    }

    string getStringName(const string &raw) {
        map<string, pair<string, string> >::iterator it = stringPool.find(raw);
        if (it != stringPool.end()) {
            return it->second.first;
        }
        ostringstream oss;
        oss << "@.str." << stringPool.size();
        string name = oss.str();
        stringPool[raw] = make_pair(name, llvmStringLiteral(raw));
        return name;
    }

    static string llvmStringLiteral(const string &raw) {
        ostringstream oss;
        for (size_t i = 0; i < raw.size(); ++i) {
            unsigned char ch = static_cast<unsigned char>(raw[i]);
            if (ch == '\\') {
                if (i + 1 >= raw.size()) {
                    throw runtime_error("Invalid string escape");
                }
                if (raw[i + 1] != 'n') {
                    throw runtime_error("Only \\n is allowed in format strings");
                }
                oss << "\\0A";
                ++i;
                continue;
            }
            if (ch == '"') {
                oss << "\\22";
                continue;
            }
            if (ch == '\\') {
                oss << "\\5C";
                continue;
            }
            if (ch < 32 || ch > 126) {
                oss << "\\" << uppercase << hex << setw(2) << setfill('0')
                    << static_cast<int>(ch) << nouppercase << dec << setfill(' ');
                continue;
            }
            oss << static_cast<char>(ch);
        }
        oss << "\\00";
        return oss.str();
    }

    static int llvmStringLength(const string &raw) {
        int len = 1;
        for (size_t i = 0; i < raw.size(); ++i) {
            if (raw[i] == '\\' && i + 1 < raw.size() && raw[i + 1] == 'n') {
                ++len;
                ++i;
            } else {
                ++len;
            }
        }
        return len;
    }

    void emitStringGlobals(vector<string> &lines) {
        for (map<string, pair<string, string> >::const_iterator it = stringPool.begin();
             it != stringPool.end(); ++it) {
            int len = llvmStringLength(it->first);
            ostringstream oss;
            oss << it->second.first << " = private unnamed_addr constant ["
                << len << " x i8] c\"" << it->second.second << "\"";
            lines.push_back(oss.str());
        }
        if (!stringPool.empty()) {
            lines.push_back("");
        }
    }

    void emitConstArrayTemplateGlobals(vector<string> &lines) const {
        for (size_t i = 0; i < constArrayTemplateDefs.size(); ++i) {
            lines.push_back(constArrayTemplateDefs[i]);
        }
        if (!constArrayTemplateDefs.empty()) {
            lines.push_back("");
        }
    }

    string stringPtrExpr(const string &raw) {
        string name = getStringName(raw);
        int len = llvmStringLength(raw);
        ostringstream oss;
        oss << "getelementptr inbounds ([" << len << " x i8], [" << len << " x i8]* " << name
            << ", i32 0, i32 0)";
        return oss.str();
    }

    static BaseType arithmeticResultType(BaseType lhs, BaseType rhs) {
        return (lhs == BaseType::Float || rhs == BaseType::Float) ? BaseType::Float : BaseType::Int;
    }

    static bool constIsTrue(const ConstEvalValue &value) {
        return value.type == BaseType::Float ? value.value != 0.0 : static_cast<int>(value.value) != 0;
    }

    ConstEvalValue evalConstExpr(const shared_ptr<Expr> &expr) const {
        if (dynamic_cast<NumberExpr *>(expr.get()) != NULL) {
            shared_ptr<NumberExpr> n = static_pointer_cast<NumberExpr>(expr);
            ConstEvalValue value;
            value.type = n->type;
            value.value = n->value;
            return value;
        }
        if (dynamic_cast<UnaryExpr *>(expr.get()) != NULL) {
            shared_ptr<UnaryExpr> u = static_pointer_cast<UnaryExpr>(expr);
            ConstEvalValue rhs = evalConstExpr(u->operand);
            if (u->op == TokenKind::Plus) {
                return rhs;
            }
            if (u->op == TokenKind::Minus) {
                rhs.value = -rhs.value;
                return rhs;
            }
            ConstEvalValue result;
            result.type = BaseType::Int;
            result.value = constIsTrue(rhs) ? 0 : 1;
            return result;
        }
        if (dynamic_cast<BinaryExpr *>(expr.get()) != NULL) {
            shared_ptr<BinaryExpr> b = static_pointer_cast<BinaryExpr>(expr);
            ConstEvalValue lhs = evalConstExpr(b->lhs);
            ConstEvalValue rhs = evalConstExpr(b->rhs);
            BaseType arithType = arithmeticResultType(lhs.type, rhs.type);
            double lval = lhs.value;
            double rval = rhs.value;
            ConstEvalValue result;
            switch (b->op) {
                case TokenKind::Plus:
                    result.type = arithType;
                    result.value = lval + rval;
                    return result;
                case TokenKind::Minus:
                    result.type = arithType;
                    result.value = lval - rval;
                    return result;
                case TokenKind::Mul:
                    result.type = arithType;
                    result.value = lval * rval;
                    return result;
                case TokenKind::Div:
                    result.type = arithType;
                    result.value = arithType == BaseType::Float ? lval / rval
                                                                : static_cast<int>(lval) / static_cast<int>(rval);
                    return result;
                case TokenKind::Mod:
                    result.type = BaseType::Int;
                    result.value = static_cast<int>(lval) % static_cast<int>(rval);
                    return result;
                case TokenKind::Lt:
                    result.type = BaseType::Int;
                    result.value = lval < rval ? 1 : 0;
                    return result;
                case TokenKind::Gt:
                    result.type = BaseType::Int;
                    result.value = lval > rval ? 1 : 0;
                    return result;
                case TokenKind::Le:
                    result.type = BaseType::Int;
                    result.value = lval <= rval ? 1 : 0;
                    return result;
                case TokenKind::Ge:
                    result.type = BaseType::Int;
                    result.value = lval >= rval ? 1 : 0;
                    return result;
                case TokenKind::Eq:
                    result.type = BaseType::Int;
                    result.value = lval == rval ? 1 : 0;
                    return result;
                case TokenKind::Ne:
                    result.type = BaseType::Int;
                    result.value = lval != rval ? 1 : 0;
                    return result;
                case TokenKind::And:
                    result.type = BaseType::Int;
                    result.value = (constIsTrue(lhs) && constIsTrue(rhs)) ? 1 : 0;
                    return result;
                case TokenKind::Or:
                    result.type = BaseType::Int;
                    result.value = (constIsTrue(lhs) || constIsTrue(rhs)) ? 1 : 0;
                    return result;
                default: break;
            }
        }
        if (dynamic_cast<LValExpr *>(expr.get()) != NULL) {
            LVal lval = static_pointer_cast<LValExpr>(expr)->lval;
            const ConstValue *cv = findConst(lval.name);
            if (cv == NULL) {
                throw runtime_error("Const expression references non-const identifier: " + lval.name);
            }
            if (!cv->isArray) {
                ConstEvalValue value;
                value.type = cv->baseType;
                value.value = cv->scalar;
                return value;
            }
            if (lval.indices.size() != cv->dims.size()) {
                throw runtime_error("Const array reference needs full indices: " + lval.name);
            }
            int index = 0;
            for (size_t i = 0; i < cv->dims.size(); ++i) {
                int sub = static_cast<int>(evalConstExpr(lval.indices[i]).value);
                index = index * cv->dims[i] + sub;
            }
            ConstEvalValue value;
            value.type = cv->baseType;
            value.value = cv->values[index];
            return value;
        }
        throw runtime_error("Unsupported const expression");
    }

    vector<int> evalDims(const vector<shared_ptr<Expr> > &dimsExpr) const {
        vector<int> dims;
        for (size_t i = 0; i < dimsExpr.size(); ++i) {
            dims.push_back(static_cast<int>(evalConstExpr(dimsExpr[i]).value));
        }
        return dims;
    }

    size_t fillConstArray(const shared_ptr<InitVal> &init,
                          const vector<int> &dims,
                          size_t level,
                          vector<double> &out,
                          size_t start) const {
        if (level == dims.size()) {
            if (start >= out.size()) {
                throw runtime_error("Too many initializer elements");
            }
            if (!init->isExpr) {
                if (init->list.empty()) {
                    return start;
                }
                return fillConstArray(init->list[0], dims, dims.size(), out, start);
            }
            out[start] = evalConstExpr(init->expr).value;
            return start + 1;
        }
        if (init->isExpr) {
            if (start >= out.size()) {
                throw runtime_error("Too many initializer elements");
            }
            out[start] = evalConstExpr(init->expr).value;
            return start + 1;
        }
        size_t pos = start;
        size_t end = start + static_cast<size_t>(product(dims, level));
        for (size_t i = 0; i < init->list.size() && pos < end; ++i) {
            if (init->list[i]->isExpr) {
                pos = fillConstArray(init->list[i], dims, dims.size(), out, pos);
            } else {
                size_t targetLevel = level + 1;
                while (targetLevel < dims.size()) {
                    size_t targetSize = static_cast<size_t>(product(dims, targetLevel));
                    if ((pos - start) % targetSize == 0) {
                        break;
                    }
                    ++targetLevel;
                }
                size_t subStart = pos;
                pos = fillConstArray(init->list[i], dims, targetLevel, out, subStart);
                if (targetLevel < dims.size()) {
                    pos = subStart + static_cast<size_t>(product(dims, targetLevel));
                }
            }
        }
        return pos;
    }

    vector<double> buildConstArrayValues(const vector<int> &dims, const shared_ptr<InitVal> &init) const {
        vector<double> values(static_cast<size_t>(product(dims)), 0.0);
        fillConstArray(init, dims, 0, values, 0);
        return values;
    }

    bool tryBuildConstArrayValues(const vector<int> &dims,
                                  const shared_ptr<InitVal> &init,
                                  vector<double> &values) const {
        if (!init) {
            return false;
        }
        try {
            values = buildConstArrayValues(dims, init);
            return true;
        } catch (const exception &) {
            values.clear();
            return false;
        }
    }

    string constArrayLiteralRec(BaseType baseType,
                                const vector<int> &dims,
                                const vector<double> &values,
                                size_t level,
                                size_t &offset,
                                bool includeType) const {
        if (level == dims.size()) {
            return llvmType(baseType) + " " + scalarLiteral(baseType, values[offset++]);
        }
        vector<string> parts;
        for (int i = 0; i < dims[level]; ++i) {
            parts.push_back(constArrayLiteralRec(baseType, dims, values, level + 1, offset, true));
        }
        string body = "[" + join(parts, ", ") + "]";
        if (includeType) {
            return arrayType(baseType, dims, level) + " " + body;
        }
        return body;
    }

    string constArrayLiteral(BaseType baseType, const vector<int> &dims, const vector<double> &values) const {
        bool allZero = true;
        for (size_t i = 0; i < values.size(); ++i) {
            if (values[i] != 0) {
                allZero = false;
                break;
            }
        }
        if (allZero) {
            return "zeroinitializer";
        }
        size_t offset = 0;
        return constArrayLiteralRec(baseType, dims, values, 0, offset, false);
    }

    bool isAllZeroArray(const vector<double> &values) const {
        for (size_t i = 0; i < values.size(); ++i) {
            if (values[i] != 0) {
                return false;
            }
        }
        return true;
    }

    long long arrayByteSize(const vector<int> &dims) const {
        return static_cast<long long>(product(dims)) * 4LL;
    }

    static string int64Value(long long value) {
        ostringstream oss;
        oss << value;
        return oss.str();
    }

    string getConstArrayTemplateName(BaseType baseType,
                                     const vector<int> &dims,
                                     const vector<double> &values) {
        string arrTy = arrayType(baseType, dims);
        string literal = constArrayLiteral(baseType, dims, values);
        string key = arrTy + " " + literal;
        map<string, string>::const_iterator it = constArrayTemplatePool.find(key);
        if (it != constArrayTemplatePool.end()) {
            return it->second;
        }
        ostringstream oss;
        oss << "@.arr.const." << constArrayTemplatePool.size();
        string name = oss.str();
        constArrayTemplatePool[key] = name;
        constArrayTemplateDefs.push_back(name + " = private unnamed_addr constant " + key);
        return name;
    }

    string bitcastToI8Ptr(const string &ptr, const string &pointeeType) {
        string reg = newTemp();
        emit(reg + " = bitcast " + pointerType(pointeeType) + " " + ptr + " to i8*");
        return reg;
    }

    void emitZeroArrayFill(BaseType baseType, const string &basePtr, const vector<int> &dims) {
        string arrTy = arrayType(baseType, dims);
        string dst = bitcastToI8Ptr(basePtr, arrTy);
        emit("call i8* @memset(i8* " + dst + ", i32 0, i64 " + int64Value(arrayByteSize(dims)) + ")");
    }

    void emitConstArrayCopy(BaseType baseType,
                            const string &basePtr,
                            const vector<int> &dims,
                            const vector<double> &values) {
        string arrTy = arrayType(baseType, dims);
        string srcBase = getConstArrayTemplateName(baseType, dims, values);
        string dst = bitcastToI8Ptr(basePtr, arrTy);
        string src = bitcastToI8Ptr(srcBase, arrTy);
        emit("call i8* @memcpy(i8* " + dst + ", i8* " + src + ", i64 " + int64Value(arrayByteSize(dims)) + ")");
    }

    void emitOutputStateUpdate(const string &isNewlineValue) {
        emit("store i32 1, i32* " + internalHasOutputName());
        emit("store i32 " + isNewlineValue + ", i32* " + internalLastNewlineName());
    }

    void emitBuiltinOutputTracking(const string &callee, const vector<string> &argValues) {
        if (!isRuntimeBuiltinCall(callee)) {
            return;
        }
        if (callee == "putint" || callee == "putfloat") {
            emitOutputStateUpdate("0");
            return;
        }
        if (callee == "putarray" || callee == "putfarray") {
            emitOutputStateUpdate("1");
            return;
        }
        if (callee == "putch") {
            string isNl = newTemp();
            emit(isNl + " = icmp eq i32 " + argValues[0] + ", 10");
            string asInt = newTemp();
            emit(asInt + " = zext i1 " + isNl + " to i32");
            emitOutputStateUpdate(asInt);
        }
    }

    void flattenRuntimeInit(const shared_ptr<InitVal> &init,
                            const vector<int> &dims,
                            size_t level,
                            vector<shared_ptr<Expr> > &out,
                            size_t &pos,
                            size_t start) const {
        if (level == dims.size()) {
            if (pos >= out.size()) {
                throw runtime_error("Too many initializer elements");
            }
            if (!init->isExpr) {
                if (init->list.empty()) {
                    return;
                }
                flattenRuntimeInit(init->list[0], dims, dims.size(), out, pos, pos);
                return;
            }
            out[pos++] = init->expr;
            return;
        }
        if (init->isExpr) {
            if (pos >= out.size()) {
                throw runtime_error("Too many initializer elements");
            }
            out[pos++] = init->expr;
            return;
        }
        size_t end = start + static_cast<size_t>(product(dims, level));
        for (size_t i = 0; i < init->list.size(); ++i) {
            if (pos >= end) {
                throw runtime_error("Too many initializer elements");
            }
            if (init->list[i]->isExpr) {
                flattenRuntimeInit(init->list[i], dims, dims.size(), out, pos, pos);
            } else {
                size_t targetLevel = level + 1;
                while (targetLevel < dims.size()) {
                    size_t targetSize = static_cast<size_t>(product(dims, targetLevel));
                    if ((pos - start) % targetSize == 0) {
                        break;
                    }
                    ++targetLevel;
                }
                size_t subStart = pos;
                flattenRuntimeInit(init->list[i], dims, targetLevel, out, pos, subStart);
                if (targetLevel < dims.size()) {
                    pos = subStart + static_cast<size_t>(product(dims, targetLevel));
                }
            }
        }
    }

    BaseType inferExprType(const shared_ptr<Expr> &expr) {
        if (dynamic_cast<NumberExpr *>(expr.get()) != NULL) {
            return static_pointer_cast<NumberExpr>(expr)->type;
        }
        if (dynamic_cast<LValExpr *>(expr.get()) != NULL) {
            LVal lval = static_pointer_cast<LValExpr>(expr)->lval;
            Symbol *sym = findSymbol(lval.name);
            if (sym == NULL) {
                throw runtime_error("Unknown identifier: " + lval.name);
            }
            return sym->baseType;
        }
        if (dynamic_cast<UnaryExpr *>(expr.get()) != NULL) {
            shared_ptr<UnaryExpr> u = static_pointer_cast<UnaryExpr>(expr);
            return u->op == TokenKind::Not ? BaseType::Int : inferExprType(u->operand);
        }
        if (dynamic_cast<BinaryExpr *>(expr.get()) != NULL) {
            shared_ptr<BinaryExpr> b = static_pointer_cast<BinaryExpr>(expr);
            if (b->op == TokenKind::And || b->op == TokenKind::Or ||
                b->op == TokenKind::Lt || b->op == TokenKind::Gt ||
                b->op == TokenKind::Le || b->op == TokenKind::Ge ||
                b->op == TokenKind::Eq || b->op == TokenKind::Ne ||
                b->op == TokenKind::Mod) {
                return BaseType::Int;
            }
            return arithmeticResultType(inferExprType(b->lhs), inferExprType(b->rhs));
        }
        if (dynamic_cast<CallExpr *>(expr.get()) != NULL) {
            const string &callee = static_pointer_cast<CallExpr>(expr)->callee;
            map<string, FuncSig>::const_iterator it = funcSigs.find(callee);
            if (it == funcSigs.end()) {
                throw runtime_error("Unknown function: " + callee);
            }
            return it->second.returnType;
        }
        throw runtime_error("Unsupported expression kind");
    }

    string castValue(const TypedValue &value, BaseType targetType) {
        if (value.type == targetType) {
            return value.ir;
        }
        string reg = newTemp();
        if (value.type == BaseType::Int && targetType == BaseType::Float) {
            emit(reg + " = sitofp i32 " + value.ir + " to float");
            return reg;
        }
        if (value.type == BaseType::Float && targetType == BaseType::Int) {
            emit(reg + " = fptosi float " + value.ir + " to i32");
            return reg;
        }
        throw runtime_error("Unsupported cast");
    }

    TypedValue emitScalarExpr(const shared_ptr<Expr> &expr) {
        if (dynamic_cast<NumberExpr *>(expr.get()) != NULL) {
            shared_ptr<NumberExpr> n = static_pointer_cast<NumberExpr>(expr);
            TypedValue value;
            value.type = n->type;
            value.ir = scalarLiteral(n->type, n->value);
            return value;
        }
        if (dynamic_cast<LValExpr *>(expr.get()) != NULL) {
            AddrInfo addr = emitLValAddress(static_pointer_cast<LValExpr>(expr)->lval, true);
            if (!addr.isScalar) {
                throw runtime_error("Array value used as scalar expression");
            }
            string reg = newTemp();
            emit(reg + " = load " + llvmType(addr.baseType) + ", " +
                 pointerType(llvmType(addr.baseType)) + " " + addr.ptr);
            TypedValue value;
            value.type = addr.baseType;
            value.ir = reg;
            return value;
        }
        if (dynamic_cast<UnaryExpr *>(expr.get()) != NULL) {
            shared_ptr<UnaryExpr> u = static_pointer_cast<UnaryExpr>(expr);
            TypedValue rhs = emitScalarExpr(u->operand);
            if (u->op == TokenKind::Plus) {
                return rhs;
            }
            if (u->op == TokenKind::Minus) {
                string reg = newTemp();
                if (rhs.type == BaseType::Float) {
                    emit(reg + " = fsub float 0.000000e+00, " + rhs.ir);
                } else {
                    emit(reg + " = sub i32 0, " + rhs.ir);
                }
                TypedValue value;
                value.type = rhs.type;
                value.ir = reg;
                return value;
            }
            string cond = emitIcmpNeZero(rhs);
            string asInt = newTemp();
            emit(asInt + " = zext i1 " + cond + " to i32");
            string reg = newTemp();
            emit(reg + " = xor i32 " + asInt + ", 1");
            TypedValue value;
            value.type = BaseType::Int;
            value.ir = reg;
            return value;
        }
        if (dynamic_cast<BinaryExpr *>(expr.get()) != NULL) {
            shared_ptr<BinaryExpr> b = static_pointer_cast<BinaryExpr>(expr);
            if (b->op == TokenKind::And || b->op == TokenKind::Or) {
                return emitLogicalValue(b);
            }
            TypedValue lhsValue = emitScalarExpr(b->lhs);
            TypedValue rhsValue = emitScalarExpr(b->rhs);
            if (b->op == TokenKind::Plus || b->op == TokenKind::Minus || b->op == TokenKind::Mul ||
                b->op == TokenKind::Div || b->op == TokenKind::Mod) {
                BaseType resultType = b->op == TokenKind::Mod ? BaseType::Int
                                                              : arithmeticResultType(lhsValue.type, rhsValue.type);
                string lhs = castValue(lhsValue, resultType);
                string rhs = castValue(rhsValue, resultType);
                string reg = newTemp();
                string op;
                if (resultType == BaseType::Float) {
                    if (b->op == TokenKind::Plus) op = "fadd";
                    if (b->op == TokenKind::Minus) op = "fsub";
                    if (b->op == TokenKind::Mul) op = "fmul";
                    if (b->op == TokenKind::Div) op = "fdiv";
                    emit(reg + " = " + op + " float " + lhs + ", " + rhs);
                } else {
                    if (b->op == TokenKind::Plus) op = "add";
                    if (b->op == TokenKind::Minus) op = "sub";
                    if (b->op == TokenKind::Mul) op = "mul";
                    if (b->op == TokenKind::Div) op = "sdiv";
                    if (b->op == TokenKind::Mod) op = "srem";
                    emit(reg + " = " + op + " i32 " + lhs + ", " + rhs);
                }
                TypedValue value;
                value.type = resultType;
                value.ir = reg;
                return value;
            }
            BaseType cmpType = arithmeticResultType(lhsValue.type, rhsValue.type);
            string lhs = castValue(lhsValue, cmpType);
            string rhs = castValue(rhsValue, cmpType);
            string pred;
            if (b->op == TokenKind::Lt) pred = cmpType == BaseType::Float ? "olt" : "slt";
            if (b->op == TokenKind::Gt) pred = cmpType == BaseType::Float ? "ogt" : "sgt";
            if (b->op == TokenKind::Le) pred = cmpType == BaseType::Float ? "ole" : "sle";
            if (b->op == TokenKind::Ge) pred = cmpType == BaseType::Float ? "oge" : "sge";
            if (b->op == TokenKind::Eq) pred = cmpType == BaseType::Float ? "oeq" : "eq";
            if (b->op == TokenKind::Ne) pred = cmpType == BaseType::Float ? "one" : "ne";
            string cmp = newTemp();
            if (cmpType == BaseType::Float) {
                emit(cmp + " = fcmp " + pred + " float " + lhs + ", " + rhs);
            } else {
                emit(cmp + " = icmp " + pred + " i32 " + lhs + ", " + rhs);
            }
            string reg = newTemp();
            emit(reg + " = zext i1 " + cmp + " to i32");
            TypedValue value;
            value.type = BaseType::Int;
            value.ir = reg;
            return value;
        }
        if (dynamic_cast<CallExpr *>(expr.get()) != NULL) {
            shared_ptr<CallExpr> call = static_pointer_cast<CallExpr>(expr);
            map<string, FuncSig>::const_iterator fit = funcSigs.find(call->callee);
            if (fit == funcSigs.end()) {
                throw runtime_error("Unknown function: " + call->callee);
            }
            vector<string> args;
            vector<string> rawArgs;
            if (isRuntimeBuiltinCall(call->callee) &&
                (call->callee == "starttime" || call->callee == "stoptime")) {
                rawArgs.push_back("0");
                args.push_back("i32 0");
            }
            for (size_t i = 0; i < call->args.size(); ++i) {
                if (i < fit->second.params.size() && fit->second.params[i].isArray) {
                    string ptrArg = emitPointerArg(call->args[i], fit->second.params[i].baseType,
                                                   fit->second.params[i].dims);
                    string pointeeType = arrayParamPointeeType(fit->second.params[i].baseType,
                                                               fit->second.params[i].dims);
                    rawArgs.push_back(ptrArg);
                    args.push_back(pointerType(pointeeType) + " " + ptrArg);
                } else {
                    TypedValue scalarArg = emitScalarExpr(call->args[i]);
                    BaseType targetType = i < fit->second.params.size() ? fit->second.params[i].baseType
                                                                        : scalarArg.type;
                    string casted = castValue(scalarArg, targetType);
                    rawArgs.push_back(casted);
                    args.push_back(llvmType(targetType) + " " + casted);
                }
            }
            if (fit->second.isVoid) {
                emit("call void @" + functionIRName(call->callee) + "(" + join(args, ", ") + ")");
                emitBuiltinOutputTracking(call->callee, rawArgs);
                TypedValue value;
                value.type = BaseType::Int;
                value.ir = "0";
                return value;
            }
            string reg = newTemp();
            emit(reg + " = call " + llvmType(fit->second.returnType) + " @" +
                 functionIRName(call->callee) + "(" + join(args, ", ") + ")");
            TypedValue value;
            value.type = fit->second.returnType;
            value.ir = reg;
            return value;
        }
        throw runtime_error("Unsupported expression kind");
    }

    string emitPointerArg(const shared_ptr<Expr> &expr,
                          BaseType expectedBaseType,
                          const vector<int> &expectedDims) {
        if (dynamic_cast<LValExpr *>(expr.get()) == NULL) {
            throw runtime_error("Array argument must be an lvalue");
        }
        AddrInfo addr = emitLValAddress(static_pointer_cast<LValExpr>(expr)->lval, true);
        if (addr.isScalar) {
            throw runtime_error("Scalar used where array parameter is required");
        }
        string expectedPointeeType = arrayParamPointeeType(expectedBaseType, expectedDims);
        if (addr.pointeeType == expectedPointeeType) {
            return addr.ptr;
        }
        string casted = newTemp();
        emit(casted + " = bitcast " + pointerType(addr.pointeeType) + " " + addr.ptr +
             " to " + pointerType(expectedPointeeType));
        return casted;
    }

    string emitIcmpNeZero(const TypedValue &value) {
        string reg = newTemp();
        if (value.type == BaseType::Float) {
            emit(reg + " = fcmp one float " + value.ir + ", 0.000000e+00");
        } else {
            emit(reg + " = icmp ne i32 " + value.ir + ", 0");
        }
        return reg;
    }

    TypedValue emitLogicalValue(const shared_ptr<BinaryExpr> &expr) {
        string resultPtr = newTemp();
        emitAlloca(resultPtr + " = alloca i32");
        emit("store i32 0, i32* " + resultPtr);
        string trueLabel = newLabel("logic.true.");
        string falseLabel = newLabel("logic.false.");
        string endLabel = newLabel("logic.end.");
        emitCond(expr, trueLabel, falseLabel);
        emitLabel(trueLabel);
        emit("store i32 1, i32* " + resultPtr);
        emit("br label %" + endLabel);
        emitLabel(falseLabel);
        emit("store i32 0, i32* " + resultPtr);
        emit("br label %" + endLabel);
        emitLabel(endLabel);
        string reg = newTemp();
        emit(reg + " = load i32, i32* " + resultPtr);
        TypedValue value;
        value.type = BaseType::Int;
        value.ir = reg;
        return value;
    }

    void emitCond(const shared_ptr<Expr> &expr, const string &trueLabel, const string &falseLabel) {
        if (dynamic_cast<BinaryExpr *>(expr.get()) != NULL) {
            shared_ptr<BinaryExpr> b = static_pointer_cast<BinaryExpr>(expr);
            if (b->op == TokenKind::And) {
                string mid = newLabel("land.rhs.");
                emitCond(b->lhs, mid, falseLabel);
                emitLabel(mid);
                emitCond(b->rhs, trueLabel, falseLabel);
                return;
            }
            if (b->op == TokenKind::Or) {
                string mid = newLabel("lor.rhs.");
                emitCond(b->lhs, trueLabel, mid);
                emitLabel(mid);
                emitCond(b->rhs, trueLabel, falseLabel);
                return;
            }
        }
        TypedValue value = emitScalarExpr(expr);
        string cond = emitIcmpNeZero(value);
        emit("br i1 " + cond + ", label %" + trueLabel + ", label %" + falseLabel);
    }

    AddrInfo emitLValAddress(const LVal &lval, bool allowDecay) {
        Symbol *sym = findSymbol(lval.name);
        if (sym == NULL) {
            throw runtime_error("Unknown identifier: " + lval.name);
        }
        if (!sym->isArrayParam && sym->dims.empty()) {
            if (!lval.indices.empty()) {
                throw runtime_error("Scalar variable indexed like array: " + lval.name);
            }
            AddrInfo info;
            info.ptr = sym->addr;
            info.isScalar = true;
            info.baseType = sym->baseType;
            info.pointeeType = llvmType(sym->baseType);
            return info;
        }
        if (sym->isArrayParam) {
            int rank = sym->totalRank;
            if (static_cast<int>(lval.indices.size()) > rank) {
                throw runtime_error("Too many indices on array parameter: " + lval.name);
            }
            if (lval.indices.empty()) {
                if (!allowDecay) {
                    throw runtime_error("Array used without full indices: " + lval.name);
                }
                AddrInfo info;
                info.ptr = sym->addr;
                info.isScalar = false;
                info.baseType = sym->baseType;
                info.pointeeType = sym->paramPointeeType;
                return info;
            }
            vector<string> idxRegs;
            for (size_t i = 0; i < lval.indices.size(); ++i) {
                idxRegs.push_back(castValue(emitScalarExpr(lval.indices[i]), BaseType::Int));
            }
            string reg = newTemp();
            string inst = reg + " = getelementptr inbounds " + sym->paramPointeeType + ", " +
                          pointerType(sym->paramPointeeType) + " " + sym->addr;
            for (size_t i = 0; i < idxRegs.size(); ++i) {
                inst += ", i32 " + idxRegs[i];
            }
            emit(inst);
            AddrInfo info;
            info.ptr = reg;
            info.isScalar = (static_cast<int>(lval.indices.size()) == rank);
            info.baseType = sym->baseType;
            size_t typeStart = lval.indices.empty() ? 0 : lval.indices.size() - 1;
            info.pointeeType = info.isScalar ? llvmType(sym->baseType)
                                             : nestedArrayOrScalarType(sym->baseType, sym->dims, typeStart);
            if (!info.isScalar && !allowDecay) {
                throw runtime_error("Array used without full indices: " + lval.name);
            }
            return info;
        }
        if (static_cast<int>(lval.indices.size()) > static_cast<int>(sym->dims.size())) {
            throw runtime_error("Too many indices on array: " + lval.name);
        }
        vector<string> idxRegs;
        for (size_t i = 0; i < lval.indices.size(); ++i) {
            idxRegs.push_back(castValue(emitScalarExpr(lval.indices[i]), BaseType::Int));
        }
        string reg = newTemp();
        string arrTy = arrayType(sym->baseType, sym->dims);
        string inst = reg + " = getelementptr inbounds " + arrTy + ", " +
                      pointerType(arrTy) + " " + sym->addr + ", i32 0";
        if (idxRegs.empty()) {
            if (!allowDecay) {
                throw runtime_error("Array used without full indices: " + lval.name);
            }
            inst += ", i32 0";
            emit(inst);
            AddrInfo info;
            info.ptr = reg;
            info.isScalar = false;
            info.baseType = sym->baseType;
            info.pointeeType = nestedArrayOrScalarType(sym->baseType, sym->dims, 1);
            return info;
        }
        for (size_t i = 0; i < idxRegs.size(); ++i) {
            inst += ", i32 " + idxRegs[i];
        }
        emit(inst);
        AddrInfo info;
        info.ptr = reg;
        info.isScalar = (lval.indices.size() == sym->dims.size());
        info.baseType = sym->baseType;
        info.pointeeType = info.isScalar ? llvmType(sym->baseType)
                                         : nestedArrayOrScalarType(sym->baseType, sym->dims,
                                                                   lval.indices.size());
        if (!info.isScalar && !allowDecay) {
            throw runtime_error("Array used without full indices: " + lval.name);
        }
        return info;
    }

    void emitStoreToLVal(const LVal &lval, const TypedValue &value) {
        AddrInfo addr = emitLValAddress(lval, false);
        string casted = castValue(value, addr.baseType);
        emit("store " + llvmType(addr.baseType) + " " + casted + ", " +
             pointerType(llvmType(addr.baseType)) + " " + addr.ptr);
    }

    void addConstBinding(const string &name, const ConstValue &value) {
        constScopes.back()[name] = value;
    }

    void addSymbol(const string &name, const Symbol &sym) {
        symbolScopes.back()[name] = sym;
    }

    void emitGlobalDecls(vector<string> &lines) {
        pushScope();
        for (size_t i = 0; i < prog.globals.size(); ++i) {
            emitOneGlobalDecl(lines, prog.globals[i]);
        }
        if (!prog.globals.empty()) {
            lines.push_back("");
        }
    }

    void emitOneGlobalDecl(vector<string> &lines, const Decl &decl) {
        for (size_t i = 0; i < decl.defs.size(); ++i) {
            vector<int> dims = evalDims(decl.defs[i].dims);
            string name = globalIRName(decl.defs[i].name);
            Symbol sym;
            sym.baseType = decl.baseType;
            sym.isConst = decl.isConst;
            sym.isGlobal = true;
            sym.isArrayParam = false;
            sym.addr = name;
            sym.dims = dims;
            sym.totalRank = static_cast<int>(dims.size());
            sym.paramPointeeType = "";
            sym.hasConstScalar = false;
            if (dims.empty()) {
                double initVal = 0.0;
                if (decl.defs[i].init) {
                    initVal = evalConstExpr(decl.defs[i].init->expr).value;
                }
                ostringstream oss;
                oss << name << " = " << (decl.isConst ? "constant" : "global")
                    << " " << llvmType(decl.baseType) << " " << scalarLiteral(decl.baseType, initVal);
                lines.push_back(oss.str());
                if (decl.isConst) {
                    sym.hasConstScalar = true;
                    sym.constScalar = initVal;
                    ConstValue cv;
                    cv.baseType = decl.baseType;
                    cv.isArray = false;
                    cv.scalar = initVal;
                    addConstBinding(decl.defs[i].name, cv);
                }
            } else {
                vector<double> values(static_cast<size_t>(product(dims)), 0.0);
                if (decl.defs[i].init) {
                    values = buildConstArrayValues(dims, decl.defs[i].init);
                }
                ostringstream oss;
                oss << name << " = " << (decl.isConst ? "constant" : "global") << " "
                    << arrayType(decl.baseType, dims) << " "
                    << constArrayLiteral(decl.baseType, dims, values);
                lines.push_back(oss.str());
                if (decl.isConst) {
                    sym.constArrayValues = values;
                    ConstValue cv;
                    cv.baseType = decl.baseType;
                    cv.isArray = true;
                    cv.dims = dims;
                    cv.values = values;
                    addConstBinding(decl.defs[i].name, cv);
                }
            }
            addSymbol(decl.defs[i].name, sym);
        }
    }

    void emitFunctions(vector<string> &lines) {
        for (size_t i = 0; i < prog.funcs.size(); ++i) {
            emitOneFunction(lines, prog.funcs[i]);
            lines.push_back("");
        }
    }

    void emitMainWrapper(vector<string> &lines) {
        lines.push_back("define i32 @main() {");
        lines.push_back("entry:");
        lines.push_back("  %t.main.ret = call i32 @" + internalUserMainName() + "()");
        lines.push_back("  ret i32 %t.main.ret");
        lines.push_back("}");
    }

    void emitOneFunction(vector<string> &lines, const FuncDef &func) {
        allocas.clear();
        code.clear();
        loopLabels.clear();
        currentFuncIsVoid = func.isVoid;
        currentFuncReturnType = func.returnType;
        currentReturnLabel = newLabel("func.ret.");
        currentRetSlot.clear();
        pushScope();

        vector<string> params;
        vector<vector<int> > paramDims;
        for (size_t i = 0; i < func.params.size(); ++i) {
            vector<int> dims = evalDims(func.params[i].dims);
            paramDims.push_back(dims);
            ostringstream oss;
            if (func.params[i].isArray) {
                oss << pointerType(arrayParamPointeeType(func.params[i].baseType, dims));
            } else {
                oss << llvmType(func.params[i].baseType);
            }
            oss << " %p" << i;
            params.push_back(oss.str());
        }

        ostringstream header;
        header << "define " << (func.isVoid ? "void" : llvmType(func.returnType)) << " @"
               << functionIRName(func.name)
               << "(" << join(params, ", ") << ") {";
        lines.push_back(header.str());
        if (!func.isVoid) {
            currentRetSlot = newTemp();
            emitAlloca(currentRetSlot + " = alloca " + llvmType(func.returnType));
            emit("store " + llvmType(func.returnType) + " " + scalarLiteral(func.returnType, 0.0) +
                 ", " + pointerType(llvmType(func.returnType)) + " " + currentRetSlot);
        }

        for (size_t i = 0; i < func.params.size(); ++i) {
            const Param &param = func.params[i];
            Symbol sym;
            sym.baseType = param.baseType;
            sym.isConst = false;
            sym.isGlobal = false;
            sym.isArrayParam = param.isArray;
            sym.dims = paramDims[i];
            sym.hasConstScalar = false;
            if (param.isArray) {
                sym.addr = "%p" + intValue(static_cast<int>(i));
                sym.totalRank = 1 + static_cast<int>(param.dims.size());
                sym.paramPointeeType = arrayParamPointeeType(param.baseType, paramDims[i]);
            } else {
                string slot = newTemp();
                emitAlloca(slot + " = alloca " + llvmType(param.baseType));
                emit("store " + llvmType(param.baseType) + " %p" + intValue(static_cast<int>(i)) +
                     ", " + pointerType(llvmType(param.baseType)) + " " + slot);
                sym.addr = slot;
                sym.totalRank = 0;
                sym.paramPointeeType = "";
            }
            addSymbol(param.name, sym);
        }

        emitBlock(func.block, false);
        emit("br label %" + currentReturnLabel);
        emitLabel(currentReturnLabel);
        if (func.isVoid) {
            emit("ret void");
        } else {
            string rv = newTemp();
            emit(rv + " = load " + llvmType(func.returnType) + ", " +
                 pointerType(llvmType(func.returnType)) + " " + currentRetSlot);
            emit("ret " + llvmType(func.returnType) + " " + rv);
        }

        lines.push_back("entry:");
        for (size_t i = 0; i < allocas.size(); ++i) {
            lines.push_back(allocas[i]);
        }
        for (size_t i = 0; i < code.size(); ++i) {
            lines.push_back(code[i]);
        }
        lines.push_back("}");
        popScope();
    }

    void emitBlock(const shared_ptr<BlockStmt> &block, bool createScope) {
        if (createScope) {
            pushScope();
        }
        for (size_t i = 0; i < block->items.size(); ++i) {
            if (block->items[i].isDecl) {
                emitLocalDecl(block->items[i].decl);
            } else {
                emitStmt(block->items[i].stmt);
            }
        }
        if (createScope) {
            popScope();
        }
    }

    void emitLocalDecl(const Decl &decl) {
        for (size_t i = 0; i < decl.defs.size(); ++i) {
            vector<int> dims = evalDims(decl.defs[i].dims);
            Symbol sym;
            sym.baseType = decl.baseType;
            sym.isConst = decl.isConst;
            sym.isGlobal = false;
            sym.isArrayParam = false;
            sym.dims = dims;
            sym.totalRank = static_cast<int>(dims.size());
            sym.paramPointeeType = "";
            sym.hasConstScalar = false;
            string slot = newTemp();
            if (dims.empty()) {
                emitAlloca(slot + " = alloca " + llvmType(decl.baseType));
                sym.addr = slot;
                double constInit = 0.0;
                if (decl.isConst) {
                    constInit = evalConstExpr(decl.defs[i].init->expr).value;
                    sym.hasConstScalar = true;
                    sym.constScalar = constInit;
                    ConstValue cv;
                    cv.baseType = decl.baseType;
                    cv.isArray = false;
                    cv.scalar = constInit;
                    addConstBinding(decl.defs[i].name, cv);
                }
                if (decl.defs[i].init) {
                    TypedValue value;
                    if (decl.isConst) {
                        value.type = decl.baseType;
                        value.ir = scalarLiteral(decl.baseType, constInit);
                    } else {
                        value = emitScalarExpr(decl.defs[i].init->expr);
                    }
                    string casted = castValue(value, decl.baseType);
                    emit("store " + llvmType(decl.baseType) + " " + casted + ", " +
                         pointerType(llvmType(decl.baseType)) + " " + slot);
                }
            } else {
                emitAlloca(slot + " = alloca " + arrayType(decl.baseType, dims));
                sym.addr = slot;
                vector<double> constValues;
                bool canUseConstStore = false;
                if (decl.isConst) {
                    sym.constArrayValues = buildConstArrayValues(dims, decl.defs[i].init);
                    constValues = sym.constArrayValues;
                    canUseConstStore = true;
                    ConstValue cv;
                    cv.baseType = decl.baseType;
                    cv.isArray = true;
                    cv.dims = dims;
                    cv.values = sym.constArrayValues;
                    addConstBinding(decl.defs[i].name, cv);
                } else if (decl.defs[i].init) {
                    canUseConstStore = tryBuildConstArrayValues(dims, decl.defs[i].init, constValues);
                }
                if (decl.defs[i].init) {
                    if (canUseConstStore) {
                        emitConstArrayStore(decl.baseType, slot, dims, constValues);
                    } else {
                        emitInitArray(decl.baseType, slot, dims, decl.defs[i].init, false);
                    }
                }
            }
            addSymbol(decl.defs[i].name, sym);
        }
    }

    void emitInitArray(BaseType baseType,
                       const string &basePtr,
                       const vector<int> &dims,
                       const shared_ptr<InitVal> &init,
                       bool isConstInit) {
        emitZeroArrayFill(baseType, basePtr, dims);

        vector<shared_ptr<Expr> > slots(static_cast<size_t>(product(dims)));
        size_t pos = 0;
        flattenRuntimeInit(init, dims, 0, slots, pos, 0);
        for (size_t idx = 0; idx < slots.size(); ++idx) {
            if (!slots[idx]) {
                continue;
            }
            vector<int> indices(dims.size(), 0);
            size_t rem = idx;
            for (int d = static_cast<int>(dims.size()) - 1; d >= 0; --d) {
                indices[static_cast<size_t>(d)] = static_cast<int>(rem % static_cast<size_t>(dims[static_cast<size_t>(d)]));
                rem /= static_cast<size_t>(dims[static_cast<size_t>(d)]);
            }
            string gep = newTemp();
            string arrTy = arrayType(baseType, dims);
            string inst = gep + " = getelementptr inbounds " + arrTy + ", " +
                          pointerType(arrTy) + " " + basePtr + ", i32 0";
            for (size_t k = 0; k < indices.size(); ++k) {
                inst += ", i32 " + intValue(indices[k]);
            }
            emit(inst);
            string value = isConstInit ? scalarLiteral(baseType, evalConstExpr(slots[idx]).value)
                                       : castValue(emitScalarExpr(slots[idx]), baseType);
            emit("store " + llvmType(baseType) + " " + value + ", " +
                 pointerType(llvmType(baseType)) + " " + gep);
        }
    }

    void emitConstArrayStore(BaseType baseType,
                             const string &basePtr,
                             const vector<int> &dims,
                             const vector<double> &values) {
        if (isAllZeroArray(values)) {
            emitZeroArrayFill(baseType, basePtr, dims);
            return;
        }
        emitConstArrayCopy(baseType, basePtr, dims, values);
    }

    void emitStmt(const shared_ptr<Stmt> &stmt) {
        if (!stmt) {
            return;
        }
        if (dynamic_cast<BlockStmt *>(stmt.get()) != NULL) {
            emitBlock(static_pointer_cast<BlockStmt>(stmt), true);
            return;
        }
        if (dynamic_cast<AssignStmt *>(stmt.get()) != NULL) {
            shared_ptr<AssignStmt> s = static_pointer_cast<AssignStmt>(stmt);
            emitStoreToLVal(s->lval, emitScalarExpr(s->expr));
            return;
        }
        if (dynamic_cast<ExprStmt *>(stmt.get()) != NULL) {
            shared_ptr<ExprStmt> s = static_pointer_cast<ExprStmt>(stmt);
            if (s->expr) {
                emitScalarExpr(s->expr);
            }
            return;
        }
        if (dynamic_cast<IfStmt *>(stmt.get()) != NULL) {
            shared_ptr<IfStmt> s = static_pointer_cast<IfStmt>(stmt);
            string thenLabel = newLabel("if.then.");
            string elseLabel = s->elseStmt ? newLabel("if.else.") : "";
            string endLabel = newLabel("if.end.");
            emitCond(s->cond, thenLabel, s->elseStmt ? elseLabel : endLabel);
            emitLabel(thenLabel);
            emitStmt(s->thenStmt);
            emit("br label %" + endLabel);
            if (s->elseStmt) {
                emitLabel(elseLabel);
                emitStmt(s->elseStmt);
                emit("br label %" + endLabel);
            }
            emitLabel(endLabel);
            return;
        }
        if (dynamic_cast<WhileStmt *>(stmt.get()) != NULL) {
            shared_ptr<WhileStmt> s = static_pointer_cast<WhileStmt>(stmt);
            string condLabel = newLabel("while.cond.");
            string bodyLabel = newLabel("while.body.");
            string endLabel = newLabel("while.end.");
            emit("br label %" + condLabel);
            emitLabel(condLabel);
            emitCond(s->cond, bodyLabel, endLabel);
            loopLabels.push_back(make_pair(endLabel, condLabel));
            emitLabel(bodyLabel);
            emitStmt(s->body);
            emit("br label %" + condLabel);
            loopLabels.pop_back();
            emitLabel(endLabel);
            return;
        }
        if (dynamic_cast<BreakStmt *>(stmt.get()) != NULL) {
            if (loopLabels.empty()) {
                throw runtime_error("break used outside loop");
            }
            emit("br label %" + loopLabels.back().first);
            string next = newLabel("after.break.");
            emitLabel(next);
            return;
        }
        if (dynamic_cast<ContinueStmt *>(stmt.get()) != NULL) {
            if (loopLabels.empty()) {
                throw runtime_error("continue used outside loop");
            }
            emit("br label %" + loopLabels.back().second);
            string next = newLabel("after.continue.");
            emitLabel(next);
            return;
        }
        if (dynamic_cast<ReturnStmt *>(stmt.get()) != NULL) {
            shared_ptr<ReturnStmt> s = static_pointer_cast<ReturnStmt>(stmt);
            if (currentFuncIsVoid) {
                emit("br label %" + currentReturnLabel);
            } else {
                TypedValue value;
                if (s->expr) {
                    value = emitScalarExpr(s->expr);
                } else {
                    value.type = currentFuncReturnType;
                    value.ir = scalarLiteral(currentFuncReturnType, 0.0);
                }
                string casted = castValue(value, currentFuncReturnType);
                emit("store " + llvmType(currentFuncReturnType) + " " + casted + ", " +
                     pointerType(llvmType(currentFuncReturnType)) + " " + currentRetSlot);
                emit("br label %" + currentReturnLabel);
            }
            string next = newLabel("after.return.");
            emitLabel(next);
            return;
        }
        if (dynamic_cast<GetIntStmt *>(stmt.get()) != NULL) {
            shared_ptr<GetIntStmt> s = static_pointer_cast<GetIntStmt>(stmt);
            string reg = newTemp();
            emit(reg + " = call i32 @getint()");
            TypedValue value;
            value.type = BaseType::Int;
            value.ir = reg;
            emitStoreToLVal(s->lval, value);
            return;
        }
        if (dynamic_cast<PrintfStmt *>(stmt.get()) != NULL) {
            shared_ptr<PrintfStmt> s = static_pointer_cast<PrintfStmt>(stmt);
            vector<string> args;
            args.push_back("i8* " + stringPtrExpr(stripQuotes(s->format)));
            for (size_t i = 0; i < s->args.size(); ++i) {
                TypedValue value = emitScalarExpr(s->args[i]);
                args.push_back(llvmType(value.type) + " " + value.ir);
            }
            emit("call i32 (i8*, ...) @printf(" + join(args, ", ") + ")");
            string raw = stripQuotes(s->format);
            bool endsWithNewline = raw.size() >= 2 &&
                                   raw[raw.size() - 2] == '\\' &&
                                   raw[raw.size() - 1] == 'n';
            emitOutputStateUpdate(endsWithNewline ? "1" : "0");
            return;
        }
        throw runtime_error("Unsupported statement kind");
    }
};

string generateLlvmIr(const CompUnit &program) {
    CodeGen codegen(program);
    return codegen.generate();
}

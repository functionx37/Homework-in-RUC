#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "frontend.h"
#include "ir_codegen.h"
#include "llvm_ir.h"
#include "riscv_backend.h"

using namespace std;

struct CommandLineOptions {
    string inputPath;
    string outputPath;
};

static string usageMessage(const char *programName) {
    ostringstream oss;
    oss << "Usage: " << programName << " -S -o <output.s> <input.sy>";
    return oss.str();
}

static CommandLineOptions parseCommandLine(int argc, char *argv[]) {
    CommandLineOptions options;
    bool emitAssembly = false;
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-S") {
            emitAssembly = true;
            continue;
        }
        if (arg == "-o") {
            if (i + 1 >= argc) {
                throw runtime_error("Missing output path after -o");
            }
            options.outputPath = argv[++i];
            continue;
        }
        if (!arg.empty() && arg[0] == '-') {
            throw runtime_error("Unsupported option: " + arg);
        }
        if (!options.inputPath.empty()) {
            throw runtime_error("Multiple input files are not supported");
        }
        options.inputPath = arg;
    }
    if (!emitAssembly) {
        throw runtime_error("Missing required option -S");
    }
    if (options.outputPath.empty()) {
        throw runtime_error("Missing required option -o <output.s>");
    }
    if (options.inputPath.empty()) {
        throw runtime_error("Missing input source file");
    }
    return options;
}

static string readFile(const string &path) {
    ifstream fin(path.c_str(), ios::in | ios::binary);
    if (!fin.is_open()) {
        throw runtime_error("Cannot open input file: " + path);
    }
    stringstream buffer;
    buffer << fin.rdbuf();
    return buffer.str();
}

static void writeFile(const string &path, const string &content) {
    ofstream fout(path.c_str(), ios::out | ios::binary);
    if (!fout.is_open()) {
        throw runtime_error("Cannot open output file: " + path);
    }
    fout << content;
}

int main(int argc, char *argv[]) {
    CommandLineOptions options;
    try {
        options = parseCommandLine(argc, argv);
    } catch (const exception &ex) {
        cerr << ex.what() << endl;
        cerr << usageMessage(argv[0]) << endl;
        return 1;
    }

    try {
        CompUnit program = parseSource(readFile(options.inputPath));
        string llvmIr = generateLlvmIr(program);
        llvmir::Module module = llvmir::parseModule(llvmIr);
        string assembly = riscv::lowerModule(module);
        writeFile(options.outputPath, assembly);
        return 0;
    } catch (const exception &ex) {
        cerr << ex.what() << endl;
        return 1;
    }
}

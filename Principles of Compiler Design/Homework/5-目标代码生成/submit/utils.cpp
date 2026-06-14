#include "utils.h"

#include <cstdlib>

int parseIntLiteral(const string &text) {
    if (text.size() > 2 && text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) {
        return static_cast<int>(strtol(text.c_str(), NULL, 16));
    }
    if (text.size() > 1 && text[0] == '0') {
        return static_cast<int>(strtol(text.c_str(), NULL, 8));
    }
    return static_cast<int>(strtol(text.c_str(), NULL, 10));
}

double parseFloatLiteral(const string &text) {
    return strtod(text.c_str(), NULL);
}

string stripQuotes(const string &text) {
    if (text.size() >= 2 && text.front() == '"' && text.back() == '"') {
        return text.substr(1, text.size() - 2);
    }
    return text;
}

string join(const vector<string> &parts, const string &sep) {
    string out;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i != 0) {
            out += sep;
        }
        out += parts[i];
    }
    return out;
}

int product(const vector<int> &dims, size_t start) {
    int result = 1;
    for (size_t i = start; i < dims.size(); ++i) {
        result *= dims[i];
    }
    return result;
}

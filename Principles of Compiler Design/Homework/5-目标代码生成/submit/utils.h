#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

int parseIntLiteral(const string &text);
double parseFloatLiteral(const string &text);
string stripQuotes(const string &text);
string join(const vector<string> &parts, const string &sep);
int product(const vector<int> &dims, size_t start = 0);

#pragma once

#include "ast.h"

#include <string>

std::string generateLlvmIr(const CompUnit &program);

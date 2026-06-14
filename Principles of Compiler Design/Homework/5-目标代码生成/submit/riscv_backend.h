#pragma once

#include <string>

#include "llvm_ir.h"

namespace riscv {

std::string lowerModule(const llvmir::Module &module);

}  // namespace riscv

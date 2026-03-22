#include "fle.hpp"
#include <iomanip>
#include <iostream>

void FLE_nm(const FLEObject& obj)
{
    for (auto symbol : obj.symbols) {

        auto addr = symbol.offset;
        std::cout << std::setw(16) << std::setfill('0') << std::hex << addr;
        std::cout << " ";

        char c;
        auto sect = symbol.section;
        auto type = symbol.type;
        if (type == SymbolType::WEAK) {
            if (sect.starts_with(".text")) {
                c = 'W';
            } else {
                c = 'V';
            }
        } else {
            c = sect.at(1);
            if (type == SymbolType::GLOBAL) {
                c = std::toupper(c);
            }
        }
        std::cout << c << " ";

        std::cout << symbol.name << std::endl;
    }
}

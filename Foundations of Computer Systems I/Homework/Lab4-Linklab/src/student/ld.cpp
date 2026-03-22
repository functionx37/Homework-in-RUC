#include "fle.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <vector>

// 页大小 4KB
const size_t PAGE_SIZE = 4096;

// 获取节的前缀类别（.text, .rodata, .data, .bss）
std::string get_section_category(std::string section_name)
{
    if (section_name.starts_with(".text"))
        return ".text";
    if (section_name.starts_with(".rodata"))
        return ".rodata";
    if (section_name.starts_with(".data"))
        return ".data";
    if (section_name.starts_with(".bss"))
        return ".bss";
    return section_name;
}

// 获取类别对应的权限
uint32_t get_category_flags(const std::string& category)
{
    if (category == ".text")
        return PHF::R | PHF::X;
    if (category == ".rodata")
        return static_cast<uint32_t>(PHF::R);
    if (category == ".data" || category == ".bss")
        return PHF::R | PHF::W;
    return PHF::R | PHF::W | PHF::X;
}

// 判断节是否是 NOBITS 类型（如 .bss）
bool is_nobits_section(const SectionHeader& shdr)
{
    return shdr.type == 8 || (shdr.flags & SHF::NOBITS);
}

// 地址对齐到页边界
size_t align_to_page(size_t addr)
{
    return (addr + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE;
}

// 收集目标文件定义和引用的符号
void collect_symbols(const FLEObject& obj,
                     std::set<std::string>& defined,
                     std::set<std::string>& undefined)
{
    for (const auto& symbol : obj.symbols) {
        if (symbol.type == SymbolType::UNDEFINED) {
            undefined.insert(symbol.name);
        } else {
            defined.insert(symbol.name);
        }
    }
    
    // 从重定位中收集引用的符号
    for (const auto& [section_name, section] : obj.sections) {
        for (const auto& reloc : section.relocs) {
            // 如果符号未定义，添加到未定义集合
            if (defined.find(reloc.symbol) == defined.end()) {
                undefined.insert(reloc.symbol);
            }
        }
    }
}

// 从归档文件中查找定义了指定符号的成员
const FLEObject* find_member_defining(const FLEObject& archive,
                                       const std::string& symbol,
                                       const std::set<std::string>& already_included)
{
    for (const auto& member : archive.members) {
        // 跳过已包含的成员
        if (already_included.count(member.name)) continue;
        
        for (const auto& sym : member.symbols) {
            if (sym.name == symbol && sym.type != SymbolType::UNDEFINED) {
                return &member;
            }
        }
    }
    return nullptr;
}

FLEObject FLE_ld(const std::vector<FLEObject>& objects, const LinkerOptions& options)
{
    const size_t BASE_ADDR = 0x400000;
    bool is_shared = options.shared;

    // ========== 第一阶段：按需链接，收集所有需要的目标文件 ==========
    std::vector<FLEObject> all_objects;  // 最终需要链接的所有目标文件
    std::set<std::string> defined_symbols;   // 已定义的符号
    std::set<std::string> undefined_symbols; // 未解析的符号
    std::vector<const FLEObject*> archives;  // 归档文件列表
    std::set<std::string> included_members;  // 已包含的归档成员
    std::vector<std::string> needed_libs;    // 依赖的共享库列表（仅可执行文件模式）

    // 第一遍：处理普通目标文件，收集归档文件和共享库
    for (const auto& obj : objects) {
        if (obj.type == ".ar") {
            // 共享库模式下不处理归档文件
            if (!is_shared) {
                archives.push_back(&obj);
            }
        } else if (obj.type == ".so") {
            // 共享库文件：添加到needed列表（仅可执行文件模式）
            if (!is_shared) {
                needed_libs.push_back(obj.name);
            }
        } else if (obj.type == ".obj") {
            all_objects.push_back(obj);
            
            std::set<std::string> def, undef;
            collect_symbols(obj, def, undef);
            
            for (const auto& s : def) {
                defined_symbols.insert(s);
                undefined_symbols.erase(s);
            }
            for (const auto& s : undef) {
                if (defined_symbols.find(s) == defined_symbols.end()) {
                    undefined_symbols.insert(s);
                }
            }
        }
    }

    // 第二遍：迭代处理归档文件，按需提取成员（仅非共享库模式）
    if (!is_shared) {
        bool changed = true;
        while (changed && !undefined_symbols.empty()) {
            changed = false;
            
            for (const auto* archive : archives) {
                // 查找能解析未定义符号的成员
                std::vector<std::string> to_resolve(undefined_symbols.begin(), undefined_symbols.end());
                
                for (const auto& sym : to_resolve) {
                    const FLEObject* member = find_member_defining(*archive, sym, included_members);
                    if (member) {
                        // 找到了，提取这个成员
                        all_objects.push_back(*member);
                        included_members.insert(member->name);
                        
                        std::set<std::string> def, undef;
                        collect_symbols(*member, def, undef);
                        
                        for (const auto& s : def) {
                            defined_symbols.insert(s);
                            undefined_symbols.erase(s);
                        }
                        for (const auto& s : undef) {
                            if (defined_symbols.find(s) == defined_symbols.end()) {
                                undefined_symbols.insert(s);
                            }
                        }
                        
                        changed = true;
                    }
                }
            }
        }
    }

    // ========== 第二阶段：正常链接流程 ==========
    std::vector<std::string> category_order = { ".text", ".rodata", ".data", ".bss" };
    std::map<std::string, std::vector<uint8_t>> category_data;
    std::map<std::string, size_t> category_size;
    std::map<std::pair<size_t, std::string>, std::pair<std::string, size_t>> section_info;

    for (size_t i = 0; i < all_objects.size(); i++) {
        for (auto [section_name, section] : all_objects[i].sections) {
            std::string category = get_section_category(section_name);

            bool is_nobits = false;
            for (const auto& shdr : all_objects[i].shdrs) {
                if (shdr.name == section_name) {
                    is_nobits = is_nobits_section(shdr);
                    break;
                }
            }

            size_t offset_in_category = category_size[category];
            section_info[{ i, section_name }] = { category, offset_in_category };

            if (is_nobits) {
                for (const auto& shdr : all_objects[i].shdrs) {
                    if (shdr.name == section_name) {
                        category_size[category] += shdr.size;
                        break;
                    }
                }
            } else {
                category_data[category].insert(
                    category_data[category].end(),
                    section.data.begin(),
                    section.data.end());
                category_size[category] += section.data.size();
            }
        }
    }

    // 计算每个类别的最终地址
    std::map<std::string, size_t> category_base_addr;
    size_t current_addr = BASE_ADDR;

    for (auto cat : category_order) {
        if (category_size.find(cat) != category_size.end() && category_size[cat] > 0) {
            category_base_addr[cat] = current_addr;
            current_addr += category_size[cat];
            current_addr = align_to_page(current_addr);
        }
    }

    // 建立全局符号表
    std::map<std::string, size_t> symbol_table;
    std::map<std::string, SymbolType> symbol_types;
    std::map<std::string, Symbol> resolved_symbols;  // 记录决议后的全局符号信息（用于动态符号表）
    size_t entry_point = 0;

    for (size_t i = 0; i < all_objects.size(); i++) {
        for (auto symbol : all_objects[i].symbols) {
            if (symbol.type != SymbolType::UNDEFINED) {
                auto it = section_info.find({ i, symbol.section });
                if (it == section_info.end()) continue;
                auto [category, offset_in_category] = it->second;

                size_t category_base = category_base_addr[category];
                size_t symbol_addr = category_base + offset_in_category + symbol.offset;

                std::string resolved_name = symbol.name;
                if (symbol.type == SymbolType::LOCAL) {
                    resolved_name = all_objects[i].name + "::" + symbol.name;
                }

                auto existing = symbol_table.find(resolved_name);
                if (existing != symbol_table.end()) {
                    SymbolType existing_type = symbol_types[resolved_name];

                    if (symbol.type == SymbolType::GLOBAL && existing_type == SymbolType::GLOBAL) {
                        throw std::runtime_error("Multiple definition of strong symbol: " + symbol.name);
                    } else if (symbol.type == SymbolType::GLOBAL && existing_type == SymbolType::WEAK) {
                        symbol_table[resolved_name] = symbol_addr;
                        symbol_types[resolved_name] = SymbolType::GLOBAL;
                        // 记录决议后的符号信息（用于动态符号表）
                        if (symbol.type == SymbolType::GLOBAL || symbol.type == SymbolType::WEAK) {
                            Symbol resolved_sym = symbol;
                            resolved_sym.section = category;
                            resolved_sym.offset = offset_in_category + symbol.offset;
                            resolved_symbols[symbol.name] = resolved_sym;
                        }
                    }
                } else {
                    symbol_table[resolved_name] = symbol_addr;
                    symbol_types[resolved_name] = symbol.type;
                    // 记录决议后的符号信息（用于动态符号表）
                    if (symbol.type == SymbolType::GLOBAL || symbol.type == SymbolType::WEAK) {
                        Symbol resolved_sym = symbol;
                        resolved_sym.section = category;
                        resolved_sym.offset = offset_in_category + symbol.offset;
                        resolved_symbols[symbol.name] = resolved_sym;
                    }
                }

                if (symbol.name == options.entryPoint) {
                    entry_point = symbol_addr;
                }
            }
        }
    }

    // ========== Bonus 2: 为可执行文件创建GOT和PLT ==========
    std::map<std::string, size_t> got_entries;  // 外部符号 -> GOT中的索引
    std::map<std::string, size_t> plt_stubs;    // 外部函数 -> PLT stub地址
    std::vector<uint8_t> got_data;              // GOT数据（每个条目8字节）
    std::vector<uint8_t> plt_data;              // PLT数据
    size_t got_base_addr = 0;
    size_t plt_base_addr = 0;
    
    if (!is_shared && !undefined_symbols.empty()) {
        // 收集需要PLT的外部函数（有PC32重定位的外部符号）
        std::set<std::string> external_functions;
        for (size_t i = 0; i < all_objects.size(); i++) {
            for (auto [section_name, section] : all_objects[i].sections) {
                for (auto reloc : section.relocs) {
                    if (reloc.type == RelocationType::R_X86_64_PC32) {
                        std::string local_name = all_objects[i].name + "::" + reloc.symbol;
                        if (symbol_table.find(local_name) == symbol_table.end() && 
                            symbol_table.find(reloc.symbol) == symbol_table.end() &&
                            undefined_symbols.count(reloc.symbol)) {
                            external_functions.insert(reloc.symbol);
                        }
                    }
                }
            }
        }
        
        // 为所有外部符号分配GOT条目
        std::vector<std::string> external_symbols_list(undefined_symbols.begin(), undefined_symbols.end());
        for (size_t i = 0; i < external_symbols_list.size(); i++) {
            got_entries[external_symbols_list[i]] = i;
            got_data.insert(got_data.end(), 8, 0);  // 每个条目8字节，初始化为0
        }
        
        // 为外部函数生成PLT stub（先创建占位符）
        if (!external_functions.empty()) {
            // 为每个外部函数生成6字节的PLT stub（占位符，等GOT地址确定后再填充）
            plt_data.insert(plt_data.end(), external_functions.size() * 6, 0);
            // 将PLT合并到.text节
            category_data[".text"].insert(category_data[".text"].end(), plt_data.begin(), plt_data.end());
            category_size[".text"] += plt_data.size();
        }
        
        // GOT放在独立的.got节（而不是合并到.data节）
        // 这样动态重定位会被正确识别为属于.got节
        
        // 重新计算地址（因为添加了PLT）
        category_base_addr.clear();
        current_addr = BASE_ADDR;
        for (auto cat : category_order) {
            if (category_size.find(cat) != category_size.end() && category_size[cat] > 0) {
                category_base_addr[cat] = current_addr;
                current_addr += category_size[cat];
                current_addr = align_to_page(current_addr);
            }
        }
        
        // GOT放在数据段之后，作为独立的节
        if (!got_data.empty()) {
            got_base_addr = current_addr;
            current_addr += got_data.size();
            current_addr = align_to_page(current_addr);
        }
        
        // 生成PLT stub代码
        if (!external_functions.empty() && !plt_data.empty() && got_base_addr != 0) {
            plt_base_addr = category_base_addr[".text"] + (category_size[".text"] - plt_data.size());
            // 现在生成PLT stub代码
            size_t plt_offset = 0;
            for (const auto& func_name : external_functions) {
                size_t stub_addr = plt_base_addr + plt_offset;
                size_t got_index = got_entries[func_name];
                size_t got_entry_addr = got_base_addr + got_index * 8;
                
                // 计算offset：从stub结束位置（stub_addr + 6）到GOT条目的偏移
                int64_t offset_diff = static_cast<int64_t>(got_entry_addr) - static_cast<int64_t>(stub_addr + 6);
                int32_t got_offset = static_cast<int32_t>(offset_diff);
                auto stub_code = generate_plt_stub(got_offset);
                
                // 更新PLT数据（在.text节中的位置）
                size_t plt_data_offset = category_size[".text"] - plt_data.size() + plt_offset;
                for (size_t j = 0; j < 6; j++) {
                    category_data[".text"][plt_data_offset + j] = stub_code[j];
                }
                
                plt_stubs[func_name] = stub_addr;
                plt_offset += 6;
            }
        }
    }

    // 处理重定位
    std::vector<Relocation> dyn_relocs;  // 动态重定位表
    
    for (size_t i = 0; i < all_objects.size(); i++) {
        for (auto [section_name, section] : all_objects[i].sections) {
            auto it = section_info.find({ i, section_name });
            if (it == section_info.end()) continue;
            
            auto [category, offset_in_category] = it->second;
            size_t section_base_addr = category_base_addr[category] + offset_in_category;

            for (auto reloc : section.relocs) {
                std::string local_name = all_objects[i].name + "::" + reloc.symbol;
                size_t symbol_addr;

                if (symbol_table.find(local_name) != symbol_table.end()) {
                    symbol_addr = symbol_table[local_name];
                } else if (symbol_table.find(reloc.symbol) != symbol_table.end()) {
                    symbol_addr = symbol_table[reloc.symbol];
                } else {
                    // 符号未找到
                    // 检查是否是外部符号（在 undefined_symbols 中）
                    if (undefined_symbols.count(reloc.symbol)) {
                        // 外部符号：通过动态链接处理
                        if (is_shared) {
                            // 共享库模式：保留为动态重定位
                            size_t reloc_addr = section_base_addr + reloc.offset;
                            dyn_relocs.push_back(Relocation {
                                .type = reloc.type,
                                .offset = reloc_addr,
                                .symbol = reloc.symbol,
                                .addend = reloc.addend
                            });
                            continue;
                        } else {
                            // 可执行文件模式：通过GOT/PLT处理
                            size_t data_offset = offset_in_category + reloc.offset;
                            size_t reloc_addr = section_base_addr + reloc.offset;
                            
                            if (reloc.type == RelocationType::R_X86_64_PC32) {
                                // PLT32重定位：重定向到PLT stub
                                if (plt_stubs.find(reloc.symbol) == plt_stubs.end()) {
                                    throw std::runtime_error("PLT stub not found for external function: " + reloc.symbol);
                                }
                                size_t plt_stub_addr = plt_stubs[reloc.symbol];
                                int64_t rel_value = (int64_t)plt_stub_addr + reloc.addend - (int64_t)reloc_addr;
                                
                                category_data[category][data_offset + 0] = rel_value & 0xFF;
                                category_data[category][data_offset + 1] = (rel_value >> 8) & 0xFF;
                                category_data[category][data_offset + 2] = (rel_value >> 16) & 0xFF;
                                category_data[category][data_offset + 3] = (rel_value >> 24) & 0xFF;
                            } else if (reloc.type == RelocationType::R_X86_64_GOTPCREL) {
                                // GOTPCREL重定位：重定向到GOT条目
                                if (got_entries.find(reloc.symbol) == got_entries.end()) {
                                    throw std::runtime_error("GOT entry not found for external symbol: " + reloc.symbol);
                                }
                                size_t got_index = got_entries[reloc.symbol];
                                size_t got_entry_addr = got_base_addr + got_index * 8;
                                int64_t rel_value = (int64_t)got_entry_addr + reloc.addend - (int64_t)reloc_addr;
                                
                                category_data[category][data_offset + 0] = rel_value & 0xFF;
                                category_data[category][data_offset + 1] = (rel_value >> 8) & 0xFF;
                                category_data[category][data_offset + 2] = (rel_value >> 16) & 0xFF;
                                category_data[category][data_offset + 3] = (rel_value >> 24) & 0xFF;
                            } else {
                                throw std::runtime_error("Unsupported relocation type for external symbol: " + reloc.symbol);
                            }
                            continue;
                        }
                    } else {
                        // 符号既不在符号表中，也不在 undefined_symbols 中，说明是未定义的符号
                        throw std::runtime_error("Undefined symbol: " + reloc.symbol);
                    }
                }

                // 内部符号：正常处理重定位
                int64_t value = symbol_addr + reloc.addend;
                size_t data_offset = offset_in_category + reloc.offset;

                if (data_offset + 4 > category_data[category].size() && 
                    reloc.type != RelocationType::R_X86_64_64) {
                    continue;
                }
                if (data_offset + 8 > category_data[category].size() && 
                    reloc.type == RelocationType::R_X86_64_64) {
                    continue;
                }

                if (reloc.type == RelocationType::R_X86_64_32 || reloc.type == RelocationType::R_X86_64_32S) {
                    category_data[category][data_offset + 0] = value & 0xFF;
                    category_data[category][data_offset + 1] = (value >> 8) & 0xFF;
                    category_data[category][data_offset + 2] = (value >> 16) & 0xFF;
                    category_data[category][data_offset + 3] = (value >> 24) & 0xFF;
                } else if (reloc.type == RelocationType::R_X86_64_PC32) {
                    int64_t rel_value = value - (int64_t)(section_base_addr + reloc.offset);
                    category_data[category][data_offset + 0] = rel_value & 0xFF;
                    category_data[category][data_offset + 1] = (rel_value >> 8) & 0xFF;
                    category_data[category][data_offset + 2] = (rel_value >> 16) & 0xFF;
                    category_data[category][data_offset + 3] = (rel_value >> 24) & 0xFF;
                } else if (reloc.type == RelocationType::R_X86_64_64) {
                    category_data[category][data_offset + 0] = value & 0xFF;
                    category_data[category][data_offset + 1] = (value >> 8) & 0xFF;
                    category_data[category][data_offset + 2] = (value >> 16) & 0xFF;
                    category_data[category][data_offset + 3] = (value >> 24) & 0xFF;
                    category_data[category][data_offset + 4] = (value >> 32) & 0xFF;
                    category_data[category][data_offset + 5] = (value >> 40) & 0xFF;
                    category_data[category][data_offset + 6] = (value >> 48) & 0xFF;
                    category_data[category][data_offset + 7] = (value >> 56) & 0xFF;
                } else if (reloc.type == RelocationType::R_X86_64_GOTPCREL) {
                    // 内部符号的GOTPCREL重定位（理论上不应该出现，但处理一下）
                    int64_t rel_value = value - (int64_t)(section_base_addr + reloc.offset);
                    category_data[category][data_offset + 0] = rel_value & 0xFF;
                    category_data[category][data_offset + 1] = (rel_value >> 8) & 0xFF;
                    category_data[category][data_offset + 2] = (rel_value >> 16) & 0xFF;
                    category_data[category][data_offset + 3] = (rel_value >> 24) & 0xFF;
                }
            }
        }
    }

    // 生成动态重定位表（可执行文件模式：GOT条目）
    if (!is_shared && !got_entries.empty() && got_base_addr != 0) {
        for (const auto& [sym_name, got_index] : got_entries) {
            size_t got_entry_addr = got_base_addr + got_index * 8;
            dyn_relocs.push_back(Relocation {
                .type = RelocationType::R_X86_64_64,
                .offset = got_entry_addr,
                .symbol = sym_name,
                .addend = 0
            });
        }
    }

    // 构造返回对象
    FLEObject output;
    output.name = options.outputFile;
    output.type = is_shared ? ".so" : ".exe";
    if (!is_shared) {
        output.entry = entry_point;
    }
    output.dyn_relocs = dyn_relocs;
    output.needed = needed_libs;

    // 生成动态符号表（仅共享库模式）
    // 动态符号表包含所有已定义的全局符号（GLOBAL和WEAK），不包括局部符号
    if (is_shared) {
        // 使用resolved_symbols来生成动态符号表（已经过冲突决议）
        for (const auto& [name, sym] : resolved_symbols) {
            output.symbols.push_back(sym);
        }
    } else {
        // 可执行文件模式：未定义的符号应该已经通过GOT/PLT处理
        // 不需要额外检查，因为如果符号未定义且不在got_entries中，会在重定位处理时报错
    }

    for (auto cat : category_order) {
        if (category_size.find(cat) != category_size.end() && category_size[cat] > 0) {
            FLESection sec;
            sec.name = cat;
            sec.data = category_data[cat];
            sec.has_symbols = false;
            output.sections[cat] = sec;

            uint32_t shf = static_cast<uint32_t>(SHF::ALLOC);
            if (cat == ".text") shf |= SHF::EXEC;
            if (cat == ".data" || cat == ".bss") shf |= SHF::WRITE;
            if (cat == ".bss") shf |= SHF::NOBITS;

            output.shdrs.push_back(SectionHeader {
                .name = cat,
                .type = (cat == ".bss") ? 8u : 1u,
                .flags = shf,
                .addr = category_base_addr[cat],
                .offset = 0,
                .size = category_size[cat] });

            output.phdrs.push_back(ProgramHeader {
                .name = cat,
                .vaddr = category_base_addr[cat],
                .size = category_size[cat],
                .flags = get_category_flags(cat) });
        }
    }

    // 添加.got节（如果存在）
    if (!is_shared && !got_data.empty()) {
        FLESection got_sec;
        got_sec.name = ".got";
        got_sec.data = got_data;
        got_sec.has_symbols = false;
        output.sections[".got"] = got_sec;

        output.shdrs.push_back(SectionHeader {
            .name = ".got",
            .type = 1u,
            .flags = static_cast<uint32_t>(SHF::ALLOC | SHF::WRITE),
            .addr = got_base_addr,
            .offset = 0,
            .size = got_data.size() });

        output.phdrs.push_back(ProgramHeader {
            .name = ".got",
            .vaddr = got_base_addr,
            .size = got_data.size(),
            .flags = PHF::R | PHF::W });
    }

    return output;
}

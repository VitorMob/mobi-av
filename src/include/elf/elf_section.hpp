#pragma once

#include "structs_elf/structs.hpp"
#include "../logger/mlogger.hpp"

class ElfSection
{
public:
    explicit ElfSection();
    ~ElfSection();

    void MapElfSection(const struct FileElf &);

    const uint32_t Name(uint16_t = 0);
    const uint32_t Type(uint16_t = 0);
    const uint32_t Link(uint16_t = 0);
    const uint32_t Info(uint16_t = 0);
    const off_t Flags(uint16_t = 0);
    const off_t Addr(uint16_t = 0);
    const off_t Addralign(uint16_t = 0);
    const off_t Entsize(uint16_t = 0);
    const off_t Offset(uint16_t = 0);
    const off_t Size(uint16_t = 0);
    const std::size_t SizeSectionHeader() const;

private:
    std::vector<Elf64_Shdr *> m_Section64;
    std::vector<Elf32_Shdr *> m_Section32;
    mLogger m_log;
    bool m_class64;
};
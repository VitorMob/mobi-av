#pragma once

#include "structs_elf/structs.hpp"
#include "../logger/mlogger.hpp"
#include <vector>

class ElfProgram
{
private:
        std::vector<Elf64_Phdr *> m_Program64;
        std::vector<Elf32_Phdr *> m_Program32;
        mLogger m_log;
        bool m_class64;

public:
        ElfProgram();
        virtual ~ElfProgram();

        void MapElfProgram(const struct FileElf &);

        const off_t Filesz(uint16_t = 0);
        const uint32_t Flags(uint16_t = 0);
        const uint32_t Type(uint16_t = 0);
        const off_t Offset(uint16_t = 0);
        const off_t Align(uint16_t = 0);
        const off_t Vaddr(uint16_t = 0);
        const off_t Paddr(uint16_t = 0);
        const std::size_t SizeProgramHeader() const;
};
#pragma once

#include "structs_elf/structs.hpp"

class ElfHeader
{

private:
    off_t m_EntryPoint;
    uint16_t m_SectionTableIndex;
    uint16_t m_SectionTableSize;
    uint16_t m_SectionTableCount;
    off_t m_SectionOffset;
    uint16_t m_Machine;
    uint16_t m_Type;
    uint16_t m_HeaderSize;
    uint32_t m_Version;
    uint16_t m_ProgramTableCount;
    uint16_t m_ProgramTableSize;
    off_t m_ProgramTableOffset;
    uint32_t m_Flags;
    unsigned char m_Class;

public:
    explicit ElfHeader();
    ~ElfHeader();

    void MapElfHeader(const struct FileElf &);

    const unsigned char Class() const;
    const off_t EntryPoint() const;
    const off_t ProgramOffset() const;
    const off_t SectionOffset() const;
    const uint32_t Version() const;
    const uint16_t HeaderSize() const;
    const uint16_t SectionTableSize() const;
    const uint16_t SectionTableCount() const;
    const uint16_t Type() const;
    const uint16_t Machine() const;
    const uint16_t ProgramTableCount() const;
    const uint16_t ProgramTableSize() const;
    const uint16_t SectionTableIndex() const;
    const uint32_t Flags() const;
};
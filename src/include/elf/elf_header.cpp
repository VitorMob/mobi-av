#if DEBUG
#include <fmt/core.h>
#endif

#include <endian.h>
#include "elf_header.hpp"

ElfHeader::ElfHeader() : m_EntryPoint(0),
                         m_SectionTableIndex(0),
                         m_SectionTableSize(0),
                         m_SectionTableCount(0),
                         m_SectionOffset(0),
                         m_Machine(0),
                         m_Type(0),
                         m_HeaderSize(0),
                         m_Version(0),
                         m_ProgramTableCount(0),
                         m_ProgramTableSize(0),
                         m_ProgramTableOffset(0),
                         m_Flags(0),
                         m_Class(0)
{
}

ElfHeader::~ElfHeader()
{
}

void ElfHeader::MapElfHeader(const struct FileElf &p_fileElf)
{
    // 64-bit
    if (p_fileElf.buffer[EI_CLASS] == ELFCLASS64)
    {
        m_Class = p_fileElf.elf.Header64->e_ident[EI_CLASS];

#if DEBUG
        fmt::print("\n[{}][DEBUG]\n\t Mapping ElfHeader File: {}\n\t Size: {}\n\t 64-bit // Swap Endian\n\t", __FUNCTION__, p_fileElf.nameFile, p_fileElf.sizeFile);
#endif
        // Header File ELF-64
        if (p_fileElf.buffer[EI_DATA] != ELFDATANATIVE)
        {
            m_EntryPoint = p_fileElf.elf.Header64->e_entry = htobe64(p_fileElf.elf.Header64->e_entry);
            m_ProgramTableOffset = p_fileElf.elf.Header64->e_phoff = htobe64(p_fileElf.elf.Header64->e_phoff);
            m_HeaderSize = p_fileElf.elf.Header64->e_ehsize = htobe16(p_fileElf.elf.Header64->e_ehsize);
            m_SectionOffset = p_fileElf.elf.Header64->e_shoff = htobe64(p_fileElf.elf.Header64->e_shoff);
            m_ProgramTableSize = p_fileElf.elf.Header64->e_phentsize = htobe16(p_fileElf.elf.Header64->e_phentsize);
            m_Flags = p_fileElf.elf.Header64->e_flags = htobe32(p_fileElf.elf.Header64->e_flags);
            m_ProgramTableCount = p_fileElf.elf.Header64->e_phnum = htobe16(p_fileElf.elf.Header64->e_phnum);
            m_Machine = p_fileElf.elf.Header64->e_machine = htobe16(p_fileElf.elf.Header64->e_machine);
            m_SectionTableIndex = p_fileElf.elf.Header64->e_shstrndx = htobe16(p_fileElf.elf.Header64->e_shstrndx);
            m_Type = p_fileElf.elf.Header64->e_type = htobe16(p_fileElf.elf.Header64->e_type);
            m_Version = p_fileElf.elf.Header64->e_version = htobe32(p_fileElf.elf.Header64->e_version);
            m_SectionTableCount = p_fileElf.elf.Header64->e_shnum = htobe16(p_fileElf.elf.Header64->e_shnum);
            m_SectionTableSize = p_fileElf.elf.Header64->e_shentsize = htobe16(p_fileElf.elf.Header64->e_shentsize);
        }
        else 
        {
            m_EntryPoint = p_fileElf.elf.Header64->e_entry;
            m_ProgramTableOffset = p_fileElf.elf.Header64->e_phoff;
            m_HeaderSize = p_fileElf.elf.Header64->e_ehsize;
            m_SectionOffset = p_fileElf.elf.Header64->e_shoff;
            m_ProgramTableSize = p_fileElf.elf.Header64->e_phentsize;
            m_Flags = p_fileElf.elf.Header64->e_flags;
            m_ProgramTableCount = p_fileElf.elf.Header64->e_phnum;
            m_Machine = p_fileElf.elf.Header64->e_machine;
            m_SectionTableIndex = p_fileElf.elf.Header64->e_shstrndx;
            m_Type = p_fileElf.elf.Header64->e_type;
            m_Version = p_fileElf.elf.Header64->e_version;
            m_SectionTableCount = p_fileElf.elf.Header64->e_shnum;
            m_SectionTableSize = p_fileElf.elf.Header64->e_shentsize;
        }
    }
    else // 32-bit
    {
        m_Class = p_fileElf.elf.Header32->e_ident[EI_CLASS];

#if DEBUG
        fmt::print("\n[{}][DEBUG]\n\t Mapping ElfHeader File: {}\n\t Size: {}\n\t 32-bit // Swap Endian", __FUNCTION__, p_fileElf.nameFile, p_fileElf.sizeFile);
#endif
        if (p_fileElf.buffer[EI_DATA] != ELFDATANATIVE)
        {
            // Header File ELF-32 
            m_EntryPoint = p_fileElf.elf.Header32->e_entry = htobe32(p_fileElf.elf.Header32->e_entry);
            m_ProgramTableOffset = p_fileElf.elf.Header32->e_phoff = htobe32(p_fileElf.elf.Header32->e_phoff);
            m_HeaderSize = p_fileElf.elf.Header32->e_ehsize = htobe16(p_fileElf.elf.Header32->e_ehsize);
            m_SectionOffset = p_fileElf.elf.Header32->e_shoff = htobe32(p_fileElf.elf.Header32->e_shoff);
            m_ProgramTableSize = p_fileElf.elf.Header32->e_phentsize = htobe16(p_fileElf.elf.Header32->e_phentsize);
            m_Flags = p_fileElf.elf.Header32->e_flags = htobe32(p_fileElf.elf.Header32->e_flags);
            m_ProgramTableCount = p_fileElf.elf.Header32->e_phnum = htobe16(p_fileElf.elf.Header32->e_phnum);
            m_Machine = p_fileElf.elf.Header32->e_machine = htobe16(p_fileElf.elf.Header32->e_machine);
            m_SectionTableIndex = p_fileElf.elf.Header32->e_shstrndx = htobe16(p_fileElf.elf.Header32->e_shstrndx);
            m_Type = p_fileElf.elf.Header32->e_type = htobe16(p_fileElf.elf.Header32->e_type);
            m_Version = p_fileElf.elf.Header32->e_version = htobe32(p_fileElf.elf.Header32->e_version);
            m_SectionTableCount = p_fileElf.elf.Header32->e_shnum = htobe16(p_fileElf.elf.Header32->e_shnum);
            m_SectionTableSize = p_fileElf.elf.Header32->e_shentsize = htobe16(p_fileElf.elf.Header32->e_shentsize);
        }
        else
        {
            m_EntryPoint = p_fileElf.elf.Header32->e_entry;
            m_ProgramTableOffset = p_fileElf.elf.Header32->e_phoff;
            m_HeaderSize = p_fileElf.elf.Header32->e_ehsize;
            m_SectionOffset = p_fileElf.elf.Header32->e_shoff;
            m_ProgramTableSize = p_fileElf.elf.Header32->e_phentsize;
            m_Flags = p_fileElf.elf.Header32->e_flags;
            m_ProgramTableCount = p_fileElf.elf.Header32->e_phnum;
            m_Machine = p_fileElf.elf.Header32->e_machine;
            m_SectionTableIndex = p_fileElf.elf.Header32->e_shstrndx;
            m_Type = p_fileElf.elf.Header32->e_type;
            m_Version = p_fileElf.elf.Header32->e_version;
            m_SectionTableCount = p_fileElf.elf.Header32->e_shnum;
            m_SectionTableSize = p_fileElf.elf.Header32->e_shentsize;
        }
    }
}

const unsigned char ElfHeader::Class() const
{
    return m_Class;
}

const  off_t ElfHeader::EntryPoint() const
{
    return m_EntryPoint;
}

const  off_t ElfHeader::ProgramOffset() const
{
    return m_ProgramTableOffset;
}

const  off_t ElfHeader::SectionOffset() const
{
    return m_SectionOffset;
}

const uint32_t ElfHeader::Version() const
{
    return m_Version;
}

const uint16_t ElfHeader::HeaderSize() const
{
    return m_HeaderSize;
}

const uint16_t ElfHeader::SectionTableSize() const
{
    return m_SectionTableSize;
}

const uint16_t ElfHeader::SectionTableCount() const
{
    return m_SectionTableCount;
}

const uint16_t ElfHeader::Type() const
{
    return m_Type;
}

const uint16_t ElfHeader::Machine() const
{
    return m_Machine;
}

const uint16_t ElfHeader::ProgramTableCount() const
{
    return m_ProgramTableCount;
}

const uint16_t ElfHeader::ProgramTableSize() const
{
    return m_ProgramTableSize;
}

const uint16_t ElfHeader::SectionTableIndex() const
{
    return m_SectionTableIndex;
}

const uint32_t ElfHeader::Flags() const
{
    return m_Flags;
}
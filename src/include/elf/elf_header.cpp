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

/**
 * @brief  Will map the Elf header of our binary
 * 
 * @param p_fileElf struct info FileElf
 */
void ElfHeader::MapElfHeader(const struct FileElf &p_fileElf)
{
    // 64-bit
    if (p_fileElf.bufferFile[EI_CLASS] == ELFCLASS64)
    {
        m_Class = p_fileElf.elf.Header64->e_ident[EI_CLASS];

#if DEBUG
        fmt::print("\n[{}][DEBUG]\n\t Mapping ElfHeader File: {}\n\t Size: {}\n\t 64-bit // Swap Endian\n\t", __FUNCTION__, p_fileElf.nameFile, p_fileElf.sizeFile);
#endif
        // Header File ELF-64
        if (p_fileElf.bufferFile[EI_DATA] != ELFDATANATIVE)
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
        if (p_fileElf.bufferFile[EI_DATA] != ELFDATANATIVE)
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

/**
 * @brief  The fifth byte identifies the architecture for this
                     binary:

                     ELFCLASSNONE
                            This class is invalid.
                     ELFCLASS32
                            This defines the 32-bit architecture.  It
                            supports machines with files and virtual
                            address spaces up to 4 Gigabytes.
                     ELFCLASS64
                            This defines the 64-bit architecture.
 * 
 * @return const unsigned char 
 */
const unsigned char ElfHeader::Class() const
{
    return m_Class;
}

/**
 * @brief This member gives the virtual address to which the system
              first transfers control, thus starting the process.  If
              the file has no associated entry point, this member holds
              zero.
 * 
 * @return const off_t 
 */
const  off_t ElfHeader::EntryPoint() const
{
    return m_EntryPoint;
}

/**
 * @brief This member holds the program header table's file offset
              in bytes.  If the file has no program header table, this
              member holds zero.
 * 
 * @return const off_t 
 */
const  off_t ElfHeader::ProgramOffset() const
{
    return m_ProgramTableOffset;
}

/**
 * @brief This member holds the section header table's file offset
              in bytes.  If the file has no section header table, this
              member holds zero.
 * 
 * @return const off_t 
 */
const  off_t ElfHeader::SectionOffset() const
{
    return m_SectionOffset;
}

/**
 * @brief This member identifies the file version:
 * 
 * @return const uint32_t 
 */
const uint32_t ElfHeader::Version() const
{
    return m_Version;
}

/**
 * @brief  This member holds the ELF header's size in bytes.
 * 
 * @return const uint16_t 
 */
const uint16_t ElfHeader::HeaderSize() const
{
    return m_HeaderSize;
}

/**
 * @brief  This member holds a sections header's size in bytes.  A
              section header is one entry in the section header table;
              all entries are the same size.

 * 
 * @return const uint16_t 
 */
const uint16_t ElfHeader::SectionTableSize() const
{
    return m_SectionTableSize;
}

/**
 * @brief  This member holds the number of entries in the section
              header table.  Thus the product of e_shentsize and e_shnum
              gives the section header table's size in bytes.  If a file
              has no section header table, e_shnum holds the value of
              zero.
 * 
 * @return const uint16_t 
 */
const uint16_t ElfHeader::SectionTableCount() const
{
    return m_SectionTableCount;
}

/**
 * @brief This member of the structure identifies the object file
              type: 

              ET_NONE
                     An unknown type.
              ET_REL A relocatable file.
              ET_EXEC
                     An executable file.
              ET_DYN A shared object.
              ET_CORE
                     A core file.
 * 
 * @return const uint16_t 
 */
const uint16_t ElfHeader::Type() const
{
    return m_Type;
}

/**
 * @brief  This member specifies the required architecture for an
              individual file.  For example:

              EM_NONE
                     An unknown machine
              EM_M32 AT&T WE 32100
              EM_SPARC
                     Sun Microsystems SPARC
              EM_386 Intel 80386
              EM_68K Motorola 68000
              EM_88K Motorola 88000
              EM_860 Intel 80860
              EM_MIPS
                     MIPS RS3000 (big-endian only)
 * 
 * @return const uint16_t 
 */
const uint16_t ElfHeader::Machine() const
{
    return m_Machine;
}

/**
 * @brief   This member holds the number of entries in the program
              header table.  Thus the product of e_phentsize and e_phnum
              gives the table's size in bytes.  If a file has no program
              header, e_phnum holds the value zero.

              If the number of entries in the program header table is
              larger than or equal to PN_XNUM (0xffff), this member
              holds PN_XNUM (0xffff) and the real number of entries in
              the program header table is held in the sh_info member of
              the initial entry in section header table.  Otherwise, the
              sh_info member of the initial entry contains the value
              zero.

              PN_XNUM
                     This is defined as 0xffff, the largest number
                     e_phnum can have, specifying where the actual
                     number of program headers is assigned.
 * 
 * @return const uint16_t 
 */
const uint16_t ElfHeader::ProgramTableCount() const
{
    return m_ProgramTableCount;
}

/**
 * @brief This member holds the size in bytes of one entry in the
              file's program header table; all entries are the same
              size.
 * 
 * @return const uint16_t 
 */
const uint16_t ElfHeader::ProgramTableSize() const
{
    return m_ProgramTableSize;
}

/**
 * @brief  This member holds the section header table index of the
              entry associated with the section name string table.  If
              the file has no section name string table, this member
              holds the value SHN_UNDEF.

              If the index of section name string table section is
              larger than or equal to SHN_LORESERVE (0xff00), this
              member holds SHN_XINDEX (0xffff) and the real index of the
              section name string table section is held in the sh_link
              member of the initial entry in section header table.
              Otherwise, the sh_link member of the initial entry in
              section header table contains the value zero.
 * 
 * @return const uint16_t 
 */
const uint16_t ElfHeader::SectionTableIndex() const
{
    return m_SectionTableIndex;
}

/**
 * @brief  This member holds processor-specific flags associated with
              the file.  Flag names take the form EF_`machine_flag'.
              Currently, no flags have been defined.
 * 
 * @return const uint32_t 
 */
const uint32_t ElfHeader::Flags() const
{
    return m_Flags;
}
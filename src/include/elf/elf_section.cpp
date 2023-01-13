#include "elf_section.hpp"
#include <string>
#include <stdexcept>
#include <fmt/core.h>

// optimization active vetorization
#if __AVX__ && !DEBUG
#pragma GCC optimize("O2", "unroll-loops", "omit-frame-pointer", "inline") // Optimization flags
#pragma GCC option("arch=native", "tune=native", "no-zero-upper")          // Enable AVX
#pragma GCC target("avx")                                                  // Enable AVX
#include <x86intrin.h>                                                     // AVX/SSE Extensions
#elif __SSE__ && !DEBUG
#pragma GCC target("sse")                                                  // Enable SSE
#include <x86intrin.h>                                                     // AVX/SSE Extensions
#pragma GCC optimize("O2", "unroll-loops", "omit-frame-pointer", "inline") // Optimization flags
#pragma GCC option("arch=native", "tune=native", "no-zero-upper")          // Enable AVX
#endif

// swap struct section header for 64-bit
#define SwapEndian64Section(swap)                         \
        swap->sh_name = htobe32(swap->sh_name);           \
        swap->sh_type = htobe32(swap->sh_type);           \
        swap->sh_link = htobe32(swap->sh_link);           \
        swap->sh_info = htobe32(swap->sh_info);           \
        swap->sh_flags = htobe64(swap->sh_flags);         \
        swap->sh_addr = htobe64(swap->sh_addr);           \
        swap->sh_offset = htobe64(swap->sh_offset);       \
        swap->sh_addralign = htobe64(swap->sh_addralign); \
        swap->sh_entsize = htobe64(swap->sh_entsize);     \
        swap->sh_size = htobe64(swap->sh_size);

// swap struct section header for 32-bit
#define SwapEndian32Section(swap)                         \
        swap->sh_name = htobe32(swap->sh_name);           \
        swap->sh_type = htobe32(swap->sh_type);           \
        swap->sh_link = htobe32(swap->sh_link);           \
        swap->sh_info = htobe32(swap->sh_info);           \
        swap->sh_flags = htobe32(swap->sh_flags);         \
        swap->sh_addr = htobe32(swap->sh_addr);           \
        swap->sh_addralign = htobe32(swap->sh_addralign); \
        swap->sh_entsize = htobe32(swap->sh_entsize);     \
        swap->sh_offset = htobe32(swap->sh_offset);       \
        swap->sh_size = htobe32(swap->sh_size);

ElfSection::ElfSection()
{
}

ElfSection::~ElfSection()
{
}

/**
 * @brief Will map the sections of our elf binary
 * 
 * @param p_fileElf struct info FileElf
 */
void ElfSection::MapElfSection(const struct FileElf &p_fileElf)
{
        if (p_fileElf.buffer != nullptr)
        {
                bool endian = p_fileElf.buffer[EI_DATA] != ELFDATANATIVE;

                // 64-bit
                if (p_fileElf.buffer[EI_CLASS] == ELFCLASS64)
                {
#if DEBUG
                        fmt::print("\n[{}][DEBUG]\n\t Mapping ElfSection File: {}\n\t Size: {}\n\t 64-bit \n\t", __FUNCTION__, p_fileElf.nameFile, p_fileElf.sizeFile);
#endif
                        ValidateOffset(p_fileElf.elf.Header64->e_shoff, static_cast<long unsigned int>(p_fileElf.sizeFile), "ELF : section offset not valid, invalid size");
                        ValidateOffset(p_fileElf.elf.Header64->e_shnum, static_cast<long unsigned int>(p_fileElf.sizeFile), "ELF : section counter not valid, invalid size");
                        ValidateOffset(p_fileElf.elf.Header64->e_shentsize, static_cast<long unsigned int>(p_fileElf.sizeFile), "ELF : section size not valid, invalid size");

                        p_fileElf.elf.Section64.reserve(p_fileElf.elf.Header64->e_shnum * p_fileElf.elf.Header64->e_shentsize);

                        // store section offset
                        for (int i = 0; i < p_fileElf.elf.Header64->e_phnum; i++)
                        {
                                p_fileElf.elf.Section64.push_back((Elf64_Shdr *)(p_fileElf.buffer + (p_fileElf.elf.Header64->e_shoff + (i * p_fileElf.elf.Header64->e_shentsize))));
                                if (endian)
                                {
                                        SwapEndian64Section(p_fileElf.elf.Section64.back());
                                }
                        }

                        m_Section64 = p_fileElf.elf.Section64;
                }
                // 32-bit
                else
                {
#if DEBUG
                        fmt::print("\n[{}][DEBUG]\n\t Mapping ElfSection File: {}\n\t Size: {}\n\t 32-bit \n\t", __FUNCTION__, p_fileElf.nameFile, p_fileElf.sizeFile);
#endif
                        ValidateOffset(p_fileElf.elf.Header32->e_shoff, static_cast<long unsigned int>(p_fileElf.sizeFile), "ELF : section offset not valid, invalid size");
                        ValidateOffset(p_fileElf.elf.Header32->e_shnum, static_cast<long unsigned int>(p_fileElf.sizeFile), "ELF : section counter not valid, invalid size");
                        ValidateOffset(p_fileElf.elf.Header32->e_shstrndx, static_cast<long unsigned int>(p_fileElf.sizeFile), "ELF : section offset not valid, invalid size");

                        p_fileElf.elf.Section32.reserve(p_fileElf.elf.Header32->e_shnum * p_fileElf.elf.Header32->e_shentsize);

                        // store section offset
                        for (int i = 0; i < p_fileElf.elf.Header32->e_phnum; i++)
                        {
                                p_fileElf.elf.Section32.push_back((Elf32_Shdr *)(p_fileElf.buffer + (p_fileElf.elf.Header32->e_shoff + (i * p_fileElf.elf.Header32->e_shentsize))));
                                if (endian)
                                {
                                        SwapEndian32Section(p_fileElf.elf.Section32.back());
                                }
                        }

                        m_Section32 = p_fileElf.elf.Section32;
                }

                (m_Section64.size() > 0) ? m_class64 = true : m_class64 = false;
        }
        else
        {
                std::string msg = "ELF : buffer invalid, check if the elf has been parsed";
                m_log.mlogger_msg(ERROR, msg);
                throw std::logic_error(msg);
        }
}

/**
 * @brief  This member specifies the name of the section.  Its value 
 *         is an index into the section header string table section,
           giving the location of a null-terminated string.
 * 
 * @param p_pos position 
 * @return const uint32_t 
 */
const uint32_t ElfSection::Name(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_name : m_Section32.at(p_pos)->sh_name;
}

/**
 * @brief member specifies the sections's semantics.
 * 
 * @param p_pos position
 * @return const uint32_t 
 */
const uint32_t ElfSection::Type(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_type : m_Section32.at(p_pos)->sh_type;
}

/**
 * @brief This member holds a section header table index link, whose 
 *         interpretation depends on the section type.
 * 
 * @param p_pos  position
 * @return const uint32_t 
 */
const uint32_t ElfSection::Link(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_link : m_Section32.at(p_pos)->sh_link;
}

/**
 * @brief  This member holds extra information, whose interpretation depends on the section type.
 * 
 * @param p_pos  position
 * @return const uint32_t 
 */
const uint32_t ElfSection::Info(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_info : m_Section32.at(p_pos)->sh_info;
}

/**
 * @brief  Sections support one-bit flags that describe miscellaneous
           attributes.  If a flag bit is set in sh_flags, the
           attribute is "on" for the section.  Otherwise, the
           attribute is "off" or does not apply.  Undefined
           attributes are set to zero.
 * 
 * @param p_pos position
 * @return const off_t 
 */
const off_t ElfSection::Flags(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_flags : m_Section32.at(p_pos)->sh_flags;
}

/**
 * @brief  If this section appears in the memory image of a process,
           this member holds the address at which the section's first
           byte should reside.  Otherwise, the member contains zero.
 * 
 * @param p_pos position
 * @return const off_t 
 */
const off_t ElfSection::Addr(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_addr : m_Section32.at(p_pos)->sh_addr;
}

/**
 * @brief Some sections have address alignment constraints.  If a
          section holds a doubleword, the system must ensure
          doubleword alignment for the entire section.  That is, the
          value of sh_addr must be congruent to zero, modulo the
          value of sh_addralign.  Only zero and positive integral
          powers of two are allowed.  The value 0 or 1 means that
          the section has no alignment constraints.
 * 
 * @param p_pos  position
 * @return const off_t 
 */
const off_t ElfSection::Addralign(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_addralign : m_Section32.at(p_pos)->sh_addralign;
}

/**
 * @brief  Some sections hold a table of fixed-sized entries, such as
           a symbol table.  For such a section, this member gives the
           size in bytes for each entry.  This member contains zero
           if the section does not hold a table of fixed-size
           entries.
 * 
 * @param p_pos position
 * @return const off_t 
 */
const off_t ElfSection::Entsize(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_entsize : m_Section32.at(p_pos)->sh_entsize;
}

/**
 * @brief  This member's value holds the byte offset from the
           beginning of the file to the first byte in the section.
           One section type, SHT_NOBITS, occupies no space in the
           file, and its sh_offset member locates the conceptual
           placement in the file.
 * 
 * @param p_pos position
 * @return const off_t 
 */
const off_t ElfSection::Offset(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_offset : m_Section32.at(p_pos)->sh_offset;
}

/**
 * @brief   This member holds the section's size in bytes.  Unless the
            section type is SHT_NOBITS, the section occupies sh_size
            bytes in the file.  A section of type SHT_NOBITS may have
            a nonzero size, but it occupies no space in the file.
 * 
 * @param p_pos position
 * @return const off_t 
 */
const off_t ElfSection::Size(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_size : m_Section32.at(p_pos)->sh_size;
}

/**
 * @brief Get size Section Header
 * 
 * @return const std::size_t 
 */
const std::size_t ElfSection::SizeSectionHeader() const
{
        return (m_class64) ? m_Section64.size() : m_Section32.size();
}
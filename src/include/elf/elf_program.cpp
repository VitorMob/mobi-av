
#if DEBUG
#include <fmt/core.h>
#endif
#include <stdexcept>
#include <endian.h>

#include "elf_program.hpp"

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

// swap struct program header for 64-bit
#define SwapEndian64Program(swap)                 \
        swap->p_offset = htobe64(swap->p_offset); \
        swap->p_filesz = htobe64(swap->p_filesz); \
        swap->p_flags = htobe32(swap->p_flags);   \
        swap->p_paddr = htobe64(swap->p_paddr);   \
        swap->p_vaddr = htobe64(swap->p_vaddr);   \
        swap->p_align = htobe64(swap->p_align);   \
        swap->p_type = htobe32(swap->p_type);

// swap struct program header for 32-bit
#define SwapEndian32Program(swap)                 \
        swap->p_offset = htobe32(swap->p_offset); \
        swap->p_filesz = htobe32(swap->p_filesz); \
        swap->p_flags = htobe32(swap->p_flags);   \
        swap->p_paddr = htobe32(swap->p_paddr);   \
        swap->p_vaddr = htobe32(swap->p_vaddr);   \
        swap->p_align = htobe32(swap->p_align);   \
        swap->p_type = htobe32(swap->p_type);

ElfProgram::ElfProgram() : m_class64(true)
{
}

ElfProgram::~ElfProgram()
{
}

/**
 * @brief  Will map the Program of our elf binary
 * 
 * @param p_fileElf struct info FileElf
 */
void ElfProgram::MapElfProgram(const struct FileElf &p_fileElf)
{
        if (p_fileElf.bufferFile != nullptr)
        {
                bool endian = p_fileElf.bufferFile[EI_DATA] != ELFDATANATIVE;

                // 64-bit
                if (p_fileElf.bufferFile[EI_CLASS] == ELFCLASS64)
                {
#if DEBUG
                        fmt::print("\n[{}][DEBUG]\n\t Mapping ElfProgram File: {}\n\t Size: {}\n\t 64-bit \n\t", __FUNCTION__, p_fileElf.nameFile, p_fileElf.sizeFile);
#endif
                        // valid offsets
                        ValidateOffset(p_fileElf.elf.Header64->e_phnum, static_cast<long unsigned int>(p_fileElf.sizeFile), "ELF : program entry count not valid");
                        ValidateOffset(p_fileElf.elf.Header64->e_phentsize, static_cast<long unsigned int>(p_fileElf.sizeFile), "ELF : program size not valid, invalid size");
                        ValidateOffset(p_fileElf.elf.Header64->e_phoff, static_cast<long unsigned int>(p_fileElf.sizeFile), "ELF : program offset not valid, invalid size");

                        p_fileElf.elf.Program64.reserve(p_fileElf.elf.Header64->e_phnum * p_fileElf.elf.Header64->e_phentsize);

                        // store program offset
                        for (int i = 0; i < p_fileElf.elf.Header64->e_phnum; i++)
                        {
                                p_fileElf.elf.Program64.push_back((Elf64_Phdr *)(p_fileElf.bufferFile + (p_fileElf.elf.Header64->e_phoff + (i * p_fileElf.elf.Header64->e_phentsize))));
                                if (endian)
                                {
                                        SwapEndian64Program(p_fileElf.elf.Program64.back());
                                }
                        }

                        m_Program64 = p_fileElf.elf.Program64;
                }
                else // 32-bit
                {
#if DEBUG
                        fmt::print("\n[{}][DEBUG]\n\t Mapping ElfProgram File: {}\n\t Size: {}\n\t 32-bit \n\t", __FUNCTION__, p_fileElf.nameFile, p_fileElf.sizeFile);
#endif
                        ValidateOffset(p_fileElf.elf.Header32->e_phentsize, (long unsigned int)p_fileElf.sizeFile, "ELF : program size not valid, invalid size");
                        ValidateOffset(p_fileElf.elf.Header32->e_phnum, (long unsigned int)p_fileElf.sizeFile, "ELF : program entry count not valid");
                        ValidateOffset(p_fileElf.elf.Header32->e_phoff, (long unsigned int)p_fileElf.sizeFile, "ELF : program offset not valid, invalid size");

                        p_fileElf.elf.Program32.reserve(p_fileElf.elf.Header32->e_phnum * p_fileElf.elf.Header32->e_phentsize);

                        for (int i = 0; i < p_fileElf.elf.Header32->e_phnum; i++)
                        {
                                p_fileElf.elf.Program32.push_back((Elf32_Phdr *)(p_fileElf.bufferFile + (p_fileElf.elf.Header32->e_phoff + (i * p_fileElf.elf.Header32->e_phentsize))));
                                if (endian)
                                {
                                        SwapEndian32Program(p_fileElf.elf.Program32.back());
                                }
                        }

                        m_Program32 = p_fileElf.elf.Program32;
                }

                (m_Program64.size() > 0) ? m_class64 = true : m_class64 = false;
        }
        else
        {
                std::string msg = "ELF : bufferFile invalid, check if the elf has been parsed";
                m_log.mlogger_msg(ERROR, msg);
                throw std::logic_error(msg);
        }
}

/**
 * @brief  This member holds the number of bytes in the file image of
           the segment.  It may be zero.
 * 
 * @param p_pos position
 * @return const off_t 
 */
const off_t ElfProgram::Filesz(uint16_t p_pos)
{
        return (m_class64) ? m_Program64.at(p_pos)->p_filesz : m_Program32.at(p_pos)->p_filesz;
}

/**
 * @brief  This member holds a bit mask of flags relevant to the
           segment:
              PF_X   An executable segment.
              PF_W   A writable segment.
              PF_R   A readable segment.
 * 
 * @param p_pos position
 * @return const uint32_t 
 */
const uint32_t ElfProgram::Flags(uint16_t p_pos)
{
        return (m_class64) ? m_Program64.at(p_pos)->p_flags : m_Program32.at(p_pos)->p_flags;
}

/**
 * @brief This member of the structure indicates what kind of
              segment this array element describes or how to interpret
              the array element's information.
 * 
 * @param p_pos position
 * @return const uint32_t 
 */
const uint32_t ElfProgram::Type(uint16_t p_pos)
{
        return (m_class64) ? m_Program64.at(p_pos)->p_type : m_Program32.at(p_pos)->p_type;
}

/**
 * @brief This member holds the offset from the beginning of the file at which the first byte of the segment resides.
 * 
 * @param p_pos  position
 * @return const off_t 
 */
const off_t ElfProgram::Offset(uint16_t p_pos)
{
        return (m_class64) ? m_Program64.at(p_pos)->p_offset : m_Program32.at(p_pos)->p_offset;
}

/**
 * @brief This member holds the value to which the segments are
          aligned in memory and in the file.  Loadable process
          segments must have congruent values for p_vaddr and
          p_offset, modulo the page size.  Values of zero and one
          mean no alignment is required.  Otherwise, p_align should
          be a positive, integral power of two, and p_vaddr should
          equal p_offset, modulo p_align.
 * 
 * @param p_pos position
 * @return const off_t 
 */
const off_t ElfProgram::Align(uint16_t p_pos)
{
        return (m_class64) ? m_Program64.at(p_pos)->p_align : m_Program32.at(p_pos)->p_align;
}

/**
 * @brief This member holds the virtual address at which the first
          byte of the segment resides in memory.
 * 
 * @param p_pos position
 * @return const off_t 
 */
const off_t ElfProgram::Vaddr(uint16_t p_pos)
{
        return (m_class64) ? m_Program64.at(p_pos)->p_vaddr : m_Program32.at(p_pos)->p_vaddr;
}

/**
 * @brief On systems for which physical addressing is relevant, this
          member is reserved for the segment's physical address.
          Under BSD this member is not used and must be zero.
 * 
 * @param p_pos 
 * @return const off_t 
 */
const off_t ElfProgram::Paddr(uint16_t p_pos)
{
        return (m_class64) ? m_Program64.at(p_pos)->p_paddr : m_Program32.at(p_pos)->p_paddr;
}

/**
 * @brief  Get size Program Header
 * 
 * @return const std::size_t 
 */
const std::size_t ElfProgram::SizeProgramHeader() const
{
        return (m_class64) ? m_Program64.size() : m_Program32.size();
}
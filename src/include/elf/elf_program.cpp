
#if DEBUG
#include <fmt/core.h>
#endif
#include <stdexcept>
#include <endian.h>
#include <string>

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

#define SwapEndian64Program(swap)                 \
        swap->p_offset = htobe64(swap->p_offset); \
        swap->p_filesz = htobe64(swap->p_filesz); \
        swap->p_flags = htobe32(swap->p_flags);   \
        swap->p_paddr = htobe64(swap->p_paddr);   \
        swap->p_vaddr = htobe64(swap->p_vaddr);   \
        swap->p_align = htobe64(swap->p_align);   \
        swap->p_type = htobe32(swap->p_type);

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

// T(N+e_phnum)
void ElfProgram::MapElfProgram(const struct FileElf &p_fileElf)
{
        if (p_fileElf.buffer != nullptr)
        {
                bool endian = p_fileElf.buffer[EI_DATA] != ELFDATANATIVE;

                // 64-bit
                if (p_fileElf.buffer[EI_CLASS] == ELFCLASS64)
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
                                p_fileElf.elf.Program64.push_back((Elf64_Phdr *)(p_fileElf.buffer + (p_fileElf.elf.Header64->e_phoff + (i * p_fileElf.elf.Header64->e_phentsize))));
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
                                p_fileElf.elf.Program32.push_back((Elf32_Phdr *)(p_fileElf.buffer + (p_fileElf.elf.Header32->e_phoff + (i * p_fileElf.elf.Header32->e_phentsize))));
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
                std::string msg = "ELF : buffer invalid, check if the elf has been parsed";
                m_log.mlogger_msg(ERROR, msg);
                throw std::logic_error(msg);
        }
}

const off_t ElfProgram::Filesz(uint16_t p_pos)
{
        return (m_class64) ? m_Program64.at(p_pos)->p_filesz : m_Program32.at(p_pos)->p_filesz;
}

const uint32_t ElfProgram::Flags(uint16_t p_pos)
{
        return (m_class64) ? m_Program64.at(p_pos)->p_flags : m_Program32.at(p_pos)->p_flags;
}

const uint32_t ElfProgram::Type(uint16_t p_pos)
{
        return (m_class64) ? m_Program64.at(p_pos)->p_type : m_Program32.at(p_pos)->p_type;
}

const off_t ElfProgram::Offset(uint16_t p_pos)
{
        return (m_class64) ? m_Program64.at(p_pos)->p_offset : m_Program32.at(p_pos)->p_offset;
}

const off_t ElfProgram::Align(uint16_t p_pos)
{
        return (m_class64) ? m_Program64.at(p_pos)->p_align : m_Program32.at(p_pos)->p_align;
}

const off_t ElfProgram::Vaddr(uint16_t p_pos)
{
        return (m_class64) ? m_Program64.at(p_pos)->p_vaddr : m_Program32.at(p_pos)->p_vaddr;
}

const off_t ElfProgram::Paddr(uint16_t p_pos)
{
        return (m_class64) ? m_Program64.at(p_pos)->p_paddr : m_Program32.at(p_pos)->p_paddr;
}

const std::size_t ElfProgram::SizeProgramHeader() const
{
        return (m_class64) ? m_Program64.size() : m_Program32.size();
}
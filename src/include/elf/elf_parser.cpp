#include "elf_parser.hpp"

#include <stdexcept>
#include <endian.h>
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

ElfParser::ElfParser(const char *p_path)
{
    m_fileElf.nameFile = p_path;
    if (m_fileElf.fdm.Open(m_fileElf.nameFile, PROT_READ | PROT_WRITE, MAP_PRIVATE))
    {
        m_fileElf.sizeFile = m_fileElf.fdm.Size();
        m_fileElf.bufferFile = m_fileElf.fdm.Buffer();

#if DEBUG
        fmt::print("\n[{}][DEBUG]\n\t Load File: {}\n\t Size: {}\n\t ", __FUNCTION__, m_fileElf.nameFile, m_fileElf.sizeFile);
#endif
        ElfCallSequence();
    }
    else
        throw std::logic_error("ELF : Error in mapper file " + std::string(m_fileElf.nameFile));
}

ElfParser::ElfParser()
{
}

ElfParser::~ElfParser()
{
}

void ElfParser::Parser(const char *p_path)
{
    m_fileElf.nameFile = p_path;
    if (m_fileElf.fdm.Open(m_fileElf.nameFile, PROT_READ | PROT_WRITE, MAP_PRIVATE))
    {
        m_fileElf.sizeFile = m_fileElf.fdm.Size();
        m_fileElf.bufferFile = m_fileElf.fdm.Buffer();

#if DEBUG
        fmt::print("\n[{}][DEBUG]\n\t Load File: {}\n\t Size: {}\n\t ", __FUNCTION__, m_fileElf.nameFile, m_fileElf.sizeFile);
#endif

        ElfCallSequence();
    }
    else
        throw std::logic_error("ELF : Error in mapper file " + std::string(m_fileElf.nameFile));
}

void ElfParser::ParserPointer(const char *p_pointer, off_t p_size, const char *p_name)
{
    m_fileElf.nameFile = p_name;
    m_fileElf.sizeFile = p_size;
    m_fileElf.bufferFile = p_pointer;
}

void ElfParser::ElfClassStructs() const
{
    if (m_fileElf.bufferFile[EI_CLASS] == ELFCLASS64)
        m_fileElf.elf.Header64 = (Elf64_Ehdr *)m_fileElf.bufferFile;
    else
        m_fileElf.elf.Header32 = (Elf32_Ehdr *)m_fileElf.bufferFile;
}

void ElfParser::ElfValidate()
{
    if (m_fileElf.bufferFile[EI_MAG0] != 0x7f && m_fileElf.bufferFile[EI_MAG1] != 0x45 &&
        m_fileElf.bufferFile[EI_MAG2] != 0x4c && m_fileElf.bufferFile[EI_MAG3] != 0x46)
    {
        const std::string msg = fmt::format("ELF : Format magic elf not valid {}\n", m_fileElf.nameFile);
        m_log.mlogger_msg(ERROR, msg);
        throw std::logic_error(msg);
    }
    if (m_fileElf.sizeFile < 45)
    {
        const std::string msg = fmt::format("ELF : elf this small binary format {}\n", m_fileElf.nameFile);
        m_log.mlogger_msg(ERROR, msg);
        throw std::logic_error(msg);
    }
}

void ElfParser::ElfCallSequence()
{
    ElfValidate();                         // 0
    ElfClassStructs();                     // 1
    m_elfHeader.MapElfHeader(m_fileElf);   // 2
    m_elfProgram.MapElfProgram(m_fileElf); // 3
    m_elfSection.MapElfSection(m_fileElf); // 4
    m_elfDynamic.MapElfDynamic(m_fileElf); // 4
}

const FileElf* ElfParser::mFileInfo() const
{
    return &m_fileElf;
}

const ElfHeader ElfParser::mElfHeader() const
{
    return m_elfHeader;
}

ElfProgram ElfParser::mElfProgram() const
{
    return m_elfProgram;
}

ElfSection ElfParser::mElfSection() const
{
    return m_elfSection;
}
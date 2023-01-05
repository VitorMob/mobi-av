#include "elf_parser.hpp"

#include <stdexcept>
#include <endian.h>
#include <fmt/core.h>

ElfParser::ElfParser(const char *p_path)
{
    m_fileElf.nameFile = p_path;
    if (m_fileElf.fdm.Open(m_fileElf.nameFile, PROT_READ | PROT_WRITE, MAP_PRIVATE))
    {
        m_fileElf.sizeFile = m_fileElf.fdm.Size();
        m_fileElf.buffer = m_fileElf.fdm.Buffer();

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
        m_fileElf.buffer = m_fileElf.fdm.Buffer();

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
    m_fileElf.buffer = p_pointer;
}

void ElfParser::ElfClassStructs() const
{
    if (m_fileElf.buffer[EI_CLASS] == ELFCLASS64)
        m_fileElf.elf.Header64 = (Elf64_Ehdr *)m_fileElf.buffer;
    else
        m_fileElf.elf.Header32 = (Elf32_Ehdr *)m_fileElf.buffer;
}

void ElfParser::ElfValidate()
{
    if (m_fileElf.buffer[EI_MAG0] != 0x7f && m_fileElf.buffer[EI_MAG1] != 0x45 &&
        m_fileElf.buffer[EI_MAG2] != 0x4c && m_fileElf.buffer[EI_MAG3] != 0x46)
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

const Elf ElfParser::StructsElf() const
{
    return m_fileElf.elf;
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
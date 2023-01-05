#if DEBUG
#include <fmt/core.h>
#endif
#include <gtest/gtest.h>

#include "../src/include/elf/elf_parser.hpp"

class T_ElfParser : public testing::Test
{
public:
    ElfParser m_parser;

    T_ElfParser() {}
    ~T_ElfParser() {}
};

TEST_F(T_ElfParser, BigEndian)
{
    m_parser.Parser("../test/elf_files/elf-Linux-PowerPC-bash");

    // Elf Header e_ident
    EXPECT_EQ(0x10017c74, m_parser.mElfHeader().EntryPoint());
    EXPECT_EQ(52, m_parser.mElfHeader().HeaderSize());
    EXPECT_EQ(31, m_parser.mElfHeader().SectionTableCount());
    EXPECT_EQ(40, m_parser.mElfHeader().SectionTableSize());
    EXPECT_EQ(32, m_parser.mElfHeader().ProgramTableSize());
    EXPECT_EQ(20, m_parser.mElfHeader().Machine());
    EXPECT_EQ(52, m_parser.mElfHeader().ProgramOffset());
    EXPECT_EQ(952788, m_parser.mElfHeader().SectionOffset());
    EXPECT_EQ(30, m_parser.mElfHeader().SectionTableIndex());
    EXPECT_EQ(1, m_parser.mElfHeader().Version());
    EXPECT_EQ(2, m_parser.mElfHeader().Type());
    EXPECT_EQ(0, m_parser.mElfHeader().Flags());

    uint32_t offsets_name[] = {0,0x0b, 0x13, 0x21, 0x34, 0x3e, 0x46, 0x4e, 0x5b};
    for (int i = 0; i < m_parser.mElfSection().SizeSectionHeader(); i++)
    {
        EXPECT_EQ(offsets_name[i], m_parser.mElfSection().Name(i));
    }

    // Elf Program Header
    // EXPECT_EQ(,);
}

TEST_F(T_ElfParser, LittleEndian)
{
    m_parser.Parser("../test/elf_files/64_intel_ls");

    // Elf Header e_ident
    EXPECT_EQ(0x404888, m_parser.mElfHeader().EntryPoint());
    EXPECT_EQ(64, m_parser.mElfHeader().HeaderSize());
    EXPECT_EQ(28, m_parser.mElfHeader().SectionTableCount());
    EXPECT_EQ(64, m_parser.mElfHeader().SectionTableSize());
    EXPECT_EQ(56, m_parser.mElfHeader().ProgramTableSize());
    EXPECT_EQ(62, m_parser.mElfHeader().Machine());
    EXPECT_EQ(64, m_parser.mElfHeader().ProgramOffset());
    EXPECT_EQ(108296, m_parser.mElfHeader().SectionOffset());
    EXPECT_EQ(27, m_parser.mElfHeader().SectionTableIndex());
    EXPECT_EQ(1, m_parser.mElfHeader().Version());
    EXPECT_EQ(2, m_parser.mElfHeader().Type());
    EXPECT_EQ(0, m_parser.mElfHeader().Flags());

    uint32_t offsets_name[] = {0, 11, 19, 33, 52, 62, 70, 78, 91};
    for (int i = 0; i < m_parser.mElfSection().SizeSectionHeader(); i++)
    {
        EXPECT_EQ(offsets_name[i], m_parser.mElfSection().Name(i));
    }
}
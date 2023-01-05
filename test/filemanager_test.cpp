#if DEBUG
#include <fmt/core.h>
#endif
#include <gtest/gtest.h>

#include "../src/include/fdm/file_manager.hpp"

class T_FileDescriptorManage : public testing::Test
{
public:
    FileDescriptorManage  *m_fd;

    T_FileDescriptorManage() {}
    ~T_FileDescriptorManage() {}
};

TEST_F(T_FileDescriptorManage, MapFile)
{
    m_fd = new FileDescriptorManage(false);

    ASSERT_TRUE(m_fd->Open("../test/elf_files/64_intel_ls", PROT_READ | PROT_WRITE, MAP_PRIVATE));

    EXPECT_EQ(m_fd->Buffer()[0], 0x7f);
    EXPECT_EQ(m_fd->Buffer()[1], 'E');
    EXPECT_EQ(m_fd->Buffer()[2], 'L');
    EXPECT_EQ(m_fd->Buffer()[3], 'F');
    EXPECT_EQ(m_fd->Size(), 110088);

    delete m_fd;
}

TEST_F(T_FileDescriptorManage, ArenaFile)
{
    m_fd = new FileDescriptorManage(true);

    ASSERT_TRUE(m_fd->Open("../test/elf_files/64_intel_ls", O_RDONLY));

    m_fd->Read();

    EXPECT_EQ(m_fd->Buffer()[0], 0x7f);
    EXPECT_EQ(m_fd->Buffer()[1], 'E');
    EXPECT_EQ(m_fd->Buffer()[2], 'L');
    EXPECT_EQ(m_fd->Buffer()[3], 'F');
    EXPECT_EQ(m_fd->Size(), 110088);

    delete m_fd;
}
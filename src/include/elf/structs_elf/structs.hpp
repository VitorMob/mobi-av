#pragma once

#include <elf.h>
#include "../../fdm/file_manager.hpp"
#include <vector>
#include <unordered_map>
#include <string>

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define ELFDATANATIVE ELFDATA2LSB
#elif __BYTE_ORDER == __BIG_ENDIAN
#define ELFDATANATIVE ELFDATA2MSB
#else
#error "Unknown machine endian"
#endif

#define ValidateOffset(offset, limit, msg) \
        if (offset > 0 && offset > limit ) \
                throw std::logic_error(msg);

struct Elf
{
    // elf header
    Elf64_Ehdr *Header64;
    Elf32_Ehdr *Header32;
   
    // program header
    std::vector<Elf64_Phdr *> Program64;
    std::vector<Elf32_Phdr *> Program32;

    // section header
    std::vector<Elf64_Shdr *> Section64;
    std::vector<Elf32_Shdr *> Section32;
    std::unordered_map<Elf64_Word, std::string> SectionTableNames;

};

struct FileElf
{
    FileDescriptorManage fdm = 0;
    const char *nameFile = nullptr;
    const char *buffer = nullptr;
    mutable off_t sizeFile = 0;
    mutable Elf elf;
};
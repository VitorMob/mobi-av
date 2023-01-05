#include "elf_section.hpp"
#include <string>
#include <stdexcept>
#include <fmt/core.h>

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

const uint32_t ElfSection::Name(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_name : m_Section32.at(p_pos)->sh_name;
}

const uint32_t ElfSection::Type(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_type : m_Section32.at(p_pos)->sh_type;
}

const uint32_t ElfSection::Link(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_link : m_Section32.at(p_pos)->sh_link;
}

const uint32_t ElfSection::Info(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_info : m_Section32.at(p_pos)->sh_info;
}

const off_t ElfSection::Flags(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_flags : m_Section32.at(p_pos)->sh_flags;
}

const off_t ElfSection::Addr(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_addr : m_Section32.at(p_pos)->sh_addr;
}

const off_t ElfSection::Addralign(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_addralign : m_Section32.at(p_pos)->sh_addralign;
}

const off_t ElfSection::Entsize(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_entsize : m_Section32.at(p_pos)->sh_entsize;
}

const off_t ElfSection::Offset(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_offset : m_Section32.at(p_pos)->sh_offset;
}

const off_t ElfSection::Size(uint16_t p_pos)
{
        return (m_class64) ? m_Section64.at(p_pos)->sh_size : m_Section32.at(p_pos)->sh_size;
}

const std::size_t ElfSection::SizeSectionHeader() const
{
        return (m_class64) ? m_Section64.size() : m_Section32.size();
}
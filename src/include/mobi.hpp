#pragma once

#include "elf/elf_parser.hpp"
#include <string>

class Mobi
{
public:
  Mobi();
  ~Mobi();

  void analyzer(const char *p_file, bool = true);
  void analyzer(std::string p_file, bool = true);

private:
  const char *m_file;
  ElfParser m_elf;
  FileDescriptorManage m_fdm;
};
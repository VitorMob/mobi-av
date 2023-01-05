#pragma once

#include "../fdm/file_manager.hpp"
#include "../logger/mlogger.hpp"
#include "structs_elf/structs.hpp"
#include "elf_header.hpp"
#include "elf_program.hpp"
#include "elf_section.hpp"
#include "elf_dynamic.hpp"

class ElfParser
{
public:
  explicit ElfParser(const char *p_path);
  explicit ElfParser();
  ~ElfParser();

  // load file elf for parser
  void Parser(const char *p_path);
  void ParserPointer(const char *, off_t, const char * = "ELF_POINTER");

  // functions for get infos this parser
  const Elf StructsElf() const;
  const ElfHeader mElfHeader() const;
  ElfProgram mElfProgram() const;
  ElfSection mElfSection() const;

private:
  struct FileElf m_fileElf;
  mLogger m_log;
  ElfHeader m_elfHeader;
  ElfProgram m_elfProgram;
  ElfSection m_elfSection;
  ElfDynamic m_elfDynamic;

  void ElfClassStructs() const;
  void ElfValidate();
  void ElfCallSequence();
};
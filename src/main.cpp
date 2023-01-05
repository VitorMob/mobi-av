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

#include <fmt/core.h>
#include "include/fdm/file_manager.hpp"
#include "include/elf/elf_parser.hpp"
#include "src/include/db/database.hpp"

int main(void)
{
    // DataBase db;

    // db.Init_db();

    ElfParser parser;

    parser.Parser("./mobi-av");
    fmt::print("Elf Header: \n\n");
    fmt::print("Class {}\n", (parser.mElfHeader().Class() == ELFCLASS64) ? "ELF64" : "ELF32");
    fmt::print("Entry Point Address 0x{:x}\n", parser.mElfHeader().EntryPoint());
    fmt::print("Program Offset File {}\n", parser.mElfHeader().ProgramOffset());
    fmt::print("ELF header size in bytes {}\n", parser.mElfHeader().HeaderSize());
    fmt::print("Section header table file offset {}\n", parser.mElfHeader().SectionOffset());
    fmt::print("Program header table entry size {}\n", parser.mElfHeader().ProgramTableSize());
    fmt::print("Flags 0x{:x}\n", parser.mElfHeader().Flags());
    fmt::print("Program header table entry count {}\n", parser.mElfHeader().ProgramTableCount());
    fmt::print("Machine {}\n", parser.mElfHeader().Machine());
    fmt::print("Section header string table index  {}\n", parser.mElfHeader().SectionTableIndex());
    fmt::print("Version  {}\n", parser.mElfHeader().Version());
    fmt::print("Section header table entry count  {}\n", parser.mElfHeader().SectionTableCount());
    fmt::print("Section header table entry size  {}\n\n", parser.mElfHeader().SectionTableSize());

    fmt::print("Program Header: \n\n");
    for (int i = 0; i < parser.mElfProgram().SizeProgramHeader(); i++)
    {
        fmt::print("Flags :{}\n", parser.mElfProgram().Flags(i));
        fmt::print("Offset :{:x}\n", parser.mElfProgram().Offset(i));
        fmt::print("Align :{}\n", parser.mElfProgram().Align(i));
        fmt::print("PAddr :{:x}\n", parser.mElfProgram().Paddr(i));
        fmt::print("Type :{}\n", parser.mElfProgram().Type(i));
        fmt::print("File :{:x}\n\n", parser.mElfProgram().Filesz(i));
    }

    for(int i = 0 ; i< parser.mElfSection().SizeSectionHeader(); i++)
    {
        fmt::print("Name : {}\n", parser.mElfSection().Name(i));
    }

    return EXIT_SUCCESS;
}
#include <benchmark/benchmark.h>
#include "../src/include/elf/elf_parser.hpp"

static void parser_elf( benchmark::State &state )
{
  // Perform setup here
  for ( auto _ : state )
  {
    // This code gets timed
    ElfParser parser_elf("./mobi-av");
  }
}

static void parser_elf_copy( benchmark::State &state )
{
  // Perform setup here
  for ( auto _ : state )
  {
    // This code gets timed
    ElfParser parser_elf("./mobi-av");
    ElfParser parser_elf2(parser_elf);

  }
}

BENCHMARK ( parser_elf );
BENCHMARK ( parser_elf_copy );

BENCHMARK_MAIN();
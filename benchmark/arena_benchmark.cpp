// ------------------------------------------------------------------
// Test BenchMark Arena-mobi
// ------------------------------------------------------------------

#include "../src/include/memory/arena.hpp"
#include <benchmark/benchmark.h>

// ------------------------------------------------------------------
// Arena
// ------------------------------------------------------------------

char lenght = sizeof(char);
Arena fast(100 * lenght);

static void Arena_Constructor(benchmark::State &state)
{
  // Perform setup here
  for (auto _ : state)
  {
    // This code gets timed
    char *p = (char *)fast.req(lenght);

    *p = 'm';
  }
}

Arena fast2(fast);
static void Arena_Copy_Constructor(benchmark::State &state)
{
  for (auto _ : state)
  {
    // This code gets timed
    char *p = (char *)fast2.req(lenght);

    *p = 'b';
  }
}

BENCHMARK(Arena_Copy_Constructor);
BENCHMARK(Arena_Constructor);

// Run the benchmark
BENCHMARK_MAIN();

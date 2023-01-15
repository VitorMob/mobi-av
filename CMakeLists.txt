cmake_minimum_required(VERSION 3.5)

project(mobi-av VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(OPTIMIZATIONS ON)
set(DEBUGGER OFF)
set(CMAKE_CXX_FLAGS ON)
set(TESTUNIT ON)
set(BENCHMARK ON)

# libs
find_package(fmt)
find_package(nlohmann_json 3.2.0 REQUIRED)

set(CMAKE_CXX_FLAGS " ")

if(OPTIMIZATIONS)
  set(CMAKE_CXX_FLAGS "-ftree-vectorize -mavx -fno-stack-protector -march=native -O2 -Os -fdelete-null-pointer-checks -fdelete-dead-exceptions")
endif()

if(DEBUGGER)
  set(CMAKE_CXX_FLAGS "-fsanitize=address -fms-extensions --all-warnings -DDEBUG -g  -O2 -Os  -Wvolatile  -Warray-bounds --extra-warnings")
endif()

if(BENCHMARK)
  find_package(benchmark REQUIRED)

  # arena
  set(PROJECT_SOURCES
    benchmark/arena_benchmark.cpp

    src/include/fdm/file_manager.cpp
    src/include/fdm/file_manager.hpp

    src/include/logger/mlogger.cpp
    src/include/logger/mlogger.hpp

    src/include/memory/arena.cpp
    src/include/memory/arena.hpp
  )

  add_executable(arena_benchmark ${PROJECT_SOURCES})
  target_link_libraries(arena_benchmark benchmark::benchmark fmt::fmt)

  # elf
  set(PROJECT_SOURCES
  
    src/include/fdm/file_manager.cpp
    src/include/fdm/file_manager.hpp

    src/include/logger/mlogger.cpp
    src/include/logger/mlogger.hpp

    benchmark/elf_benchmark.cpp

    src/include/memory/arena.cpp
    src/include/memory/arena.hpp

    src/include/fdm/file_manager.cpp
    src/include/fdm/file_manager.hpp

    src/include/elf/elf_program.cpp
    src/include/elf/elf_program.hpp

    src/include/elf/elf_header.hpp
    src/include/elf/elf_header.cpp

    src/include/elf/elf_dynamic.cpp
    src/include/elf/elf_dynamic.hpp

    src/include/elf/elf_section.cpp
    src/include/elf/elf_section.hpp

    src/include/elf/elf_parser.hpp
    src/include/elf/elf_parser.cpp
  )

  add_executable(elf_benchmark ${PROJECT_SOURCES})
  target_link_libraries(elf_benchmark benchmark::benchmark fmt::fmt)
endif()

if(TESTUNIT)
  find_package(GTest CONFIG REQUIRED)

  add_definitions(-DUNIT_TESTS)
  set(PROJECT_SOURCES
    src/include/fdm/file_manager.cpp
    src/include/fdm/file_manager.hpp

    src/include/logger/mlogger.cpp
    src/include/logger/mlogger.hpp

    test/elf_test.cpp
    test/arena_test.cpp
    test/filemanager_test.cpp

    src/include/memory/arena.cpp
    src/include/memory/arena.hpp

    src/include/fdm/file_manager.cpp
    src/include/fdm/file_manager.hpp

    src/include/mobi.cpp
    src/include/mobi.hpp

    src/include/elf/elf_header.hpp
    src/include/elf/elf_header.cpp

    src/include/elf/elf_program.cpp
    src/include/elf/elf_program.hpp

    src/include/elf/elf_dynamic.cpp
    src/include/elf/elf_dynamic.hpp

    src/include/elf/elf_section.cpp
    src/include/elf/elf_section.hpp

    src/include/elf/elf_parser.hpp
    src/include/elf/elf_parser.cpp
  )

  include(GoogleTest)
  add_executable(mobi-test ${PROJECT_SOURCES})

  target_link_libraries(mobi-test gtest gtest_main fmt::fmt nlohmann_json::nlohmann_json)
endif()

set(PROJECT_SOURCES
  src/main.cpp

  src/include/logger/mlogger.cpp
  src/include/logger/mlogger.hpp

  src/include/memory/arena.cpp
  src/include/memory/arena.hpp

  src/include/fdm/file_manager.cpp
  src/include/fdm/file_manager.hpp

  src/include/mobi.cpp
  src/include/mobi.hpp

  src/include/elf/elf_program.cpp
  src/include/elf/elf_program.hpp

  src/include/elf/elf_header.hpp
  src/include/elf/elf_header.cpp

  src/include/elf/elf_dynamic.cpp
  src/include/elf/elf_dynamic.hpp

  src/include/elf/elf_section.cpp
  src/include/elf/elf_section.hpp

  src/include/elf/elf_parser.hpp
  src/include/elf/elf_parser.cpp

  src/include/db/database.hpp
  src/include/db/database.cpp
)

add_executable(mobi-av ${PROJECT_SOURCES})
target_link_libraries(mobi-av fmt::fmt sqlite3)
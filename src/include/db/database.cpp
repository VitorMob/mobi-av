#include "database.hpp"

// optimization active vetorization
#if __AVX__ && !DEBUG
#pragma GCC optimize("O2", "unroll-loops", "omit-frame-pointer", "inline") // Optimization flags
#pragma GCC option("arch=native", "tune=native", "no-zero-upper")          // Enable AVX
#pragma GCC target("avx")                                                  // Enable AVX
#include <x86intrin.h>                                                     //AVX/SSE Extensions
#elif __SSE__ && !DEBUG
#pragma GCC target("sse")                                                  // Enable SSE
#include <x86intrin.h>                                                     //AVX/SSE Extensions
#pragma GCC optimize("O2", "unroll-loops", "omit-frame-pointer", "inline") // Optimization flags
#pragma GCC option("arch=native", "tune=native", "no-zero-upper")          // Enable AVX
#endif

#include <fmt/core.h>

DataBase::DataBase() : m_fdm(false)
{
}

DataBase::~DataBase()
{
}

void DataBase::Init_db()
{
        ParserUnixHsb();
}

/**
 * @brief parser to unix.hsb
 *
 */
void DataBase::ParserUnixHsb()
{
        if (m_fdm.Open(PATH_UNIX_DB, PROT_READ | PROT_WRITE, MAP_PRIVATE))
        {
#if DEBUG
                fmt::print("\n[{}][DEBUG]\n\t Initing Database\n\t ", __FUNCTION__);
#endif
                const char *buffer = m_fdm.Buffer();
                off_t size = m_fdm.Size();

                // Big O(N^2)
                // this vetorization intel
                for (int c = 0; c < size; c++)
                {
                        // parser  hash
                        for (int i = 0; i <= size; i++)
                        {
                                if (buffer[i] == ':')
                                        break;
                                m_malw.hash += buffer[i];
                        }

                        // parser  size
                        for (int i = m_malw.hash.size() + 1; size; i++)
                        {
                                if (buffer[i] == ':')
                                        break;
                                m_malw.size += buffer[i];
                        }

                        // parser  name file
                        for (int i = m_malw.size.size() + 1; size; i++)
                        {
                                if (buffer[i] == ':')
                                        break;
                                m_malw.name += buffer[i];
                        }

                        // clear strings
                        m_malw.size.clear();
                        m_malw.hash.clear();
                        m_malw.name.clear();
                }
        }
        else
        {
                const std::string msg = fmt::format("DataBase : error in open file {} if the file path is correct\n", PATH_UNIX_DB);
                log.mlogger_msg(ERROR, msg);
                throw std::runtime_error(msg);
        }
}
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
#include <nlohmann/json.hpp>


// for convenience
using json = nlohmann::json;

DataBase::DataBase() : m_fdm(true)
{
        if (sqlite3_open(PATH_SQLITE_DB, &m_database))
        {
                const std::string msg = fmt::format("DataBase : error in open/create database sqlite \n", PATH_SQLITE_DB);
                log.mlogger_msg(ERROR, msg);
                throw std::runtime_error(msg);
        }
        else
                CreateTables();
}

DataBase::~DataBase()
{
        sqlite3_close(m_database);
}

void DataBase::InitDb()
{
        ParserMalwareJson();
}

void DataBase::CreateTables()
{

        if (m_fdm.Open(PATH_TABLES_SQL, O_RDONLY) == true && m_fdm.Read() != nullptr)
        {
                sqlite3_exec(m_database,
                             m_fdm.Buffer(),
                             nullptr,
                             nullptr,
                             nullptr);
        }
        else
        {
                const std::string msg = fmt::format("DataBase : error in create tables {}", m_fdm.Buffer());
                log.mlogger_msg(ERROR, msg);
                throw std::runtime_error(msg);
        }
}

/**
 * @brief parser to unix.hsb
 *
 */
void DataBase::ParserMalwareJson()
{
        if (m_fdm.Open(PATH_UNIX_DB, PROT_READ | PROT_WRITE, MAP_PRIVATE) && m_fdm.Read() != nullptr && m_database)
        {
#if DEBUG
                fmt::print("\n[{}][DEBUG]\n\t Initing Database\n\t ", __FUNCTION__);
#endif
                json json_malware = json::parse(std::move(m_fdm.Buffer()));

                // iterate the array

                for (json::iterator it = json_malware.begin(); it != json_malware.end(); ++it)
                {
                        const std::string sql = fmt::format("INSERT INTO  malwares (m_hash, m_name, m_size) VALUES ({},{},{});", it.value()["Hash"].dump(), it.value()["Name"].dump(), it.value()["Size"].dump());
                        sqlite3_exec(m_database,
                                     sql.c_str(),
                                     nullptr,
                                     nullptr,
                                     nullptr);
                }
        }
        else
        {
                const std::string msg = fmt::format("DataBase : error in open file {} if the file path is correct\n", PATH_UNIX_DB);
                log.mlogger_msg(ERROR, msg);
                throw std::runtime_error(msg);
        }
}
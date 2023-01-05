#pragma once

#include "src/include/fdm/file_manager.hpp"
#include "src/include/logger/mlogger.hpp"
#include <string>

#define PATH_UNIX_DB "../src/include/db/.dbinit/unix.hsb"

class DataBase
{
public:
        DataBase();
        ~DataBase();

        void Init_db();

private:
        void ParserUnixHsb();
        FileDescriptorManage m_fdm;
        mLogger log;
        
        struct Malware
        {
                std::string hash;
                std::string name;
                std::string size;
        } m_malw;
};
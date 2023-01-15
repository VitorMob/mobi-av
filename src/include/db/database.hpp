#pragma once

#include "src/include/fdm/file_manager.hpp"
#include "src/include/logger/mlogger.hpp"
#include <string>
#include <sqlite3.h>

#define PATH_UNIX_DB "../src/include/db/.dbinit/malwares.json"
#define PATH_SQLITE_DB "../src/include/db/.dbinit/mobi-db.sql"
#define PATH_TABLES_SQL "../src/include/db/schema_tables.sql"

class DataBase
{
public:
        DataBase();
        ~DataBase();

        void InitDb();

private:
        sqlite3 *m_database;
        FileDescriptorManage m_fdm;
        mLogger log;

        void ParserMalwareJson();
        void CreateTables();

};
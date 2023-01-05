#pragma once

#include "structs_elf/structs.hpp"

class ElfDynamic
{
    public:
        explicit ElfDynamic();
        ~ElfDynamic();

        void MapElfDynamic(const struct FileElf &);        
};
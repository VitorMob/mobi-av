#include "mobi.hpp"


Mobi::Mobi() : m_file(nullptr)
{
}

Mobi::~Mobi()
{
}

void Mobi::analyzer(const char *p_file, bool p_useArena)
{
    if(p_useArena)
        return;

    p_file = p_file + '1';
}

void analyzer(std::string p_file, bool p_useArena)
{
    analyzer(p_file.c_str(), p_useArena);
}
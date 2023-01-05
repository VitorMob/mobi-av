![](assets/mobi-av-text.png)
============

> Respect the license, if you are going to use the code for private software, contact me by email mobi-av@protonmail.com

To contribute to mobi-av you need to have knowledge in C++, the code is under development, to access the documentation go to [https://mobi.com/mobi-av](https://mobi-av.com) then go to "Documentation"


I don't accept pull requests from users with the intention of changing the readme, if you want to help with the code, make a good code and understand the old one you changed, if you have memory leaks (memory leak, user-after-free, double free corruption ) or bad code performance, I will decline immediately. if you made an implementation of something in the code, make the implementation of the unit tests, this makes possible bugs to be fixed easier

Project tree contained in the [src](src/) folder
```
src/
├── include
│   ├── db
│   │   ├── database.cpp
│   │   ├── database.hpp
│   │   └── .dbinit
│   │       ├── COPYING
│   │       └── unix.hsb
│   ├── elf
│   │   ├── elf_dynamic.cpp
│   │   ├── elf_dynamic.hpp
│   │   ├── elf_header.cpp
│   │   ├── elf_header.hpp
│   │   ├── elf_parser.cpp
│   │   ├── elf_parser.hpp
│   │   ├── elf_program.cpp
│   │   ├── elf_program.hpp
│   │   ├── elf_section.cpp
│   │   ├── elf_section.hpp
│   │   └── structs_elf
│   │       └── structs.hpp
│   ├── fdm
│   │   ├── file_manager.cpp
│   │   └── file_manager.hpp
│   ├── logger
│   │   ├── config.hpp
│   │   ├── mlogger.cpp
│   │   └── mlogger.hpp
│   ├── memory
│   │   ├── arena.cpp
│   │   └── arena.hpp
│   ├── mobi.cpp
│   ├── mobi_elf.cpp
│   ├── mobi_elf.hpp
│   └── mobi.hpp
└── main.cpp

```

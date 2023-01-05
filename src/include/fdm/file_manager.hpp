#pragma once

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "../memory/arena.hpp"

#define ARENA_ALLOCATION 1000000 // 1 Mb

class FileDescriptorManage
{
public:
  FileDescriptorManage(bool p_useArena = false);
  FileDescriptorManage(const FileDescriptorManage &p_manager);
  ~FileDescriptorManage();

  FileDescriptorManage operator=(const FileDescriptorManage &p_manager);

  bool Open(const char *p_path, int p_mode);
  bool Open(const char *p_path, int p_prot, int p_flags, int p_mode = O_RDONLY);

  const char *Read(); // read for usage arena memory, if mapper memory, not necessary usage read
  off_t Size() const;
  const char *Buffer();
  ssize_t Write(const char *p_buffer, ssize_t p_size);

private:
  bool m_useArena;
  Arena m_fast;
  int m_openFile;
  const char *m_heapFile;
  off_t m_sizeFile;
  const char *m_nameFile;
  void *m_mapFile;
};
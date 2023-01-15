#include "file_manager.hpp"

#include <sys/stat.h>
#include <stdexcept>

#if DEBUG
#include <fmt/chrono.h>
#endif

FileDescriptorManage::FileDescriptorManage(bool p_useArena) : m_useArena(p_useArena),
                                                              m_openFile(0),
                                                              m_heapFile(nullptr),
                                                              m_sizeFile(0),
                                                              m_nameFile(nullptr),
                                                              m_mapFile(nullptr)
{
  if (p_useArena)
    m_fast.realloc(ARENA_ALLOCATION, ARENA_ALLOCATION);
}

FileDescriptorManage::~FileDescriptorManage()
{
  if (m_openFile > 0)
  {
    close(m_openFile);

#if DEBUG
    fmt::print("\n[{}][DEBUG] \n\t Closed File: {}\n", __FUNCTION__, m_nameFile);
#endif
  }

  if (!m_useArena && m_mapFile)
  {
    munmap(m_mapFile, m_sizeFile);

#if DEBUG
    fmt::print("\n[{}][DEBUG] \n\t Unmapped File: {}\n", __FUNCTION__, m_nameFile);
#endif
  }
}

FileDescriptorManage::FileDescriptorManage(const FileDescriptorManage &p_manager) : m_useArena(p_manager.m_useArena),
                                                                                    m_openFile(p_manager.m_openFile),
                                                                                    m_heapFile(p_manager.m_heapFile),
                                                                                    m_sizeFile(p_manager.m_sizeFile),
                                                                                    m_nameFile(p_manager.m_nameFile),
                                                                                    m_mapFile(p_manager.m_mapFile)

{
#if DEBUG
  fmt::print("\n[{}][DEBUG] \n\t Copy Constructor\n", __FUNCTION__);
#endif
}

FileDescriptorManage FileDescriptorManage::operator=(const FileDescriptorManage &p_manager)
{
  m_useArena = p_manager.m_useArena;
  m_openFile = p_manager.m_openFile;
  m_heapFile = p_manager.m_heapFile;
  m_sizeFile = p_manager.m_sizeFile;
  m_nameFile = p_manager.m_nameFile;
  m_mapFile = p_manager.m_mapFile;

#if DEBUG
  fmt::print("\n[{}][DEBUG] \n\t Copy Constructor using operator =\n", __FUNCTION__);
#endif

  return *this;
}

bool FileDescriptorManage::Open(const char *p_path, int p_mode)
{
  bool status_exit = false;

  if (m_openFile > 0)
    close(m_openFile);

  m_openFile = open(p_path, p_mode);
  struct stat statusFile;

  if (m_openFile > 0)
  {
    fstat(m_openFile, &statusFile);
    m_sizeFile = statusFile.st_size;
    m_nameFile = p_path;
    status_exit = true;

#if DEBUG
    fmt::print("\n[{}][DEBUG]\n\t File: {}\n\t Mode: {}\n", __FUNCTION__, m_nameFile, p_mode);
#endif
  }

  return status_exit;
}

bool FileDescriptorManage::Open(const char *p_path, int p_prot, int p_flags, int p_mode)
{
  bool status_exit = true;

  Open(p_path, p_mode);

  m_mapFile = mmap(nullptr, m_sizeFile, p_prot, p_flags, m_openFile, 0);

  if (m_mapFile == MAP_FAILED)
    status_exit = false;
  else
  {
#if DEBUG
    fmt::print("\n[{}][DEBUG]\n\t Mapping File: {}\n\t Mode: {}\n\t Flags: {} \n\t Protection: {}\n\t", __FUNCTION__, m_nameFile, p_mode, p_flags, p_prot);
#endif
  }

  return status_exit;
}

const char *FileDescriptorManage::Read()
{
  std::string msg;
  if (m_openFile <= 0)
  {
    msg.assign("File Manager : Error in read file, verify this opened archive is success");
    throw std::runtime_error(msg);
  }
  if (m_useArena == false)
  {
    msg.assign("File Manager : Read() method used only for unmapped files");
    throw std::runtime_error(msg);
  }

  m_heapFile = static_cast<char *>(m_fast.req(m_sizeFile));

#if DEBUG
  fmt::print("\n[{}][DEBUG]\n\t File: {}\n\t Requests Memory: {} \n\t Arena Free: {} Bytes\n\t ", __FUNCTION__, m_nameFile, m_sizeFile, m_fast.fquantity());
#endif

  if (read(m_openFile, (void *)m_heapFile, m_sizeFile) == -1)
    m_heapFile = nullptr;

  return m_heapFile;
}

ssize_t FileDescriptorManage::Write(const char *p_buffer, ssize_t p_size)
{
  if (m_openFile <= 0)
    std::runtime_error("File Manager : Error in write file, verify this opened archive is success");

#if DEBUG
  fmt::print("\n[{}][DEBUG]\n\t File: {}\n\t Writed: {} Bytes", m_nameFile, p_size);
#endif

  return write(m_openFile, p_buffer, p_size);
}

off_t FileDescriptorManage::Size() const
{
  return m_sizeFile;
}

const char *FileDescriptorManage::Buffer()
{
  return (m_useArena) ? m_heapFile : static_cast<const char *>(m_mapFile);
}
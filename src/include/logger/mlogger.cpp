#include "mlogger.hpp"

#include <ctime>
#include <fmt/chrono.h>

// get datatime (Not non-performance)  using a 5...bytes heap allocation BigO(N^2)
std::string mLogger::mcurrent_datetime(void)
{
  std::time_t tt = std::time(nullptr);
  std::tm *tm = std::localtime(&tt);
  return fmt::format("{:%Y-%m-%d.%X}", *tm);
}

mLogger::mLogger(const std::string &p_path, int p_flags)
{
  m_fileLog.Open(p_path.data(), p_flags);
}

mLogger::~mLogger()
{
}

const  std::string mLogger::mlogger_msg(const char *p_info, const std::string& p_msg)
{
  const std::string msg =  mcurrent_datetime() + std::string(p_info) + p_msg;

  m_fileLog.Write(msg.c_str(), msg.size());

  return msg;
}

void mLogger::mlogger_load_log(const char *p_path)
{
  FileDescriptorManage fileLoad;

  fileLoad.Open(p_path, O_RDONLY);
  m_fileLog.Write(fileLoad.Read(), fileLoad.Size());
}

void mLogger::mlogger_set_file(const char *p_path, int p_flags)
{
  m_fileLog.Open(p_path, p_flags);
}
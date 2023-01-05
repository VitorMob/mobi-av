#pragma once

#include <string>

#include "config.hpp"
#include "../fdm/file_manager.hpp"

class mLogger
{
public:
  explicit mLogger(const std::string & = DEFAULT_NAME_LOGGER, int = O_WRONLY | O_APPEND);
  ~mLogger();

  // functions for usage logging
  void mlogger_set_file(const char *, int = O_WRONLY | O_APPEND);
  void mlogger_load_log(const char *);
  const std::string mlogger_msg(const char *, const std::string& ); // UNDER MAINTENANCE

protected:
  std::string mcurrent_datetime(void);

private:
  FileDescriptorManage m_fileLog;

};

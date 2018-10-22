#include "utils/logger.h"

#if defined(ENABLE_LOG)
#include "log4cplus/configurator.h"
#include "log4cplus/loggingmacros.h"
#endif

#include "boost/filesystem.hpp"

namespace logger {
#if defined(ENABLE_LOG)
log4cplus::LogLevel GetLog4cplusLevel(const LogLevel& level) {
  switch (level) {
    case LogLevel::LL_DEBUG: {
      return log4cplus::DEBUG_LOG_LEVEL;
    }
    case LogLevel::LL_ERROR: {
      return log4cplus::ERROR_LOG_LEVEL;
    }
    case LogLevel::LL_FATAL: {
      return log4cplus::FATAL_LOG_LEVEL;
    }
    case LogLevel::LL_INFO: {
      return log4cplus::INFO_LOG_LEVEL;
    }
    case LogLevel::LL_TRACE: {
      return log4cplus::TRACE_LOG_LEVEL;
    }
    case LogLevel::LL_WARN: {
      return log4cplus::WARN_LOG_LEVEL;
    }
  }
  return log4cplus::NOT_SET_LOG_LEVEL;
}

void Logger::InitLogger(const std::string& ini_file_name) {
  // 3rd-party init
  log4cplus::initialize();
  log4cplus::PropertyConfigurator configurator(ini_file_name);
  configurator.configure();
}

void Logger::DeinitLogger() {
  CREATE_LOGGER("Logger");
  LOG_DEBUG("Logger deinitialization");

  // 3rd-party logger deinitialization
  log4cplus::Logger::shutdown();
}

void Logger::PushLog(const LoggerType& logger,
                     const LogLevel level,
                     const std::string& entry,
                     const LogLocation& location) {
  log4cplus::detail::macro_forced_log(logger,
                                      GetLog4cplusLevel(level),
                                      LOG4CPLUS_TEXT(entry),
                                      location.file_name_.c_str(),
                                      location.line_number_,
                                      location.function_name_.c_str());
}
#else
void Logger::InitLogger(const std::string& ini_file_name) {}
void Logger::DeinitLogger() {}
void Logger::PushLog(const LoggerType& logger,
                     const LogLevel level,
                     const std::string& entry,
                     const LogLocation& location) {}
#endif

LoggerAutoPtr::LoggerAutoPtr() {
  Logger::InitLogger("log4cplus.properties");
}

LoggerAutoPtr::~LoggerAutoPtr() {
  Logger::DeinitLogger();
}

}  // namespace logger

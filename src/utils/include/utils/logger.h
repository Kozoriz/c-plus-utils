#pragma once

#include "utils/containers/string.h"

#if defined(ENABLE_LOG)
#include "log4cplus/logger.h"
#endif

namespace logger {
#if defined(ENABLE_LOG)
typedef log4cplus::Logger LoggerType;
#else
typedef void* LoggerType;
#endif

enum class LogLevel {
  LL_TRACE,
  LL_DEBUG,
  LL_INFO,
  LL_WARN,
  LL_ERROR,
  LL_FATAL
};

struct LogLocation {
  LogLocation() {}
  LogLocation(const char* file_name,
              const char* function_name,
              const unsigned long line_number)
      : file_name_(file_name)
      , function_name_(function_name)
      , line_number_(line_number) {}

  const utils::String file_name_;
  const utils::String function_name_;
  unsigned long line_number_;
};

class Logger {
 public:
  static void InitLogger(const utils::String& log_file_name);
  static void DeinitLogger();
  static void PushLog(const logger::LoggerType& logger,
                      const LogLevel level,
                      const utils::String& entry,
                      const LogLocation& location);

 private:
  Logger() = delete;
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  ~Logger() = delete;
};

class LoggerAutoPtr {
 public:
  LoggerAutoPtr();
  ~LoggerAutoPtr();
};

struct LogMessage {
  logger::LoggerType logger_;
  logger::LogLevel level_;
  utils::String entry_;
  logger::LogLocation location_;
};

}  // namespace logger

#if defined(ENABLE_LOG)
#define CREATE_LOGGER(logger_name)             \
  static logger::LoggerType logger_module_ptr( \
      logger::LoggerType::getInstance(logger_name));

#define LOG_WITH_LEVEL(logger_var, level, message, line)    \
  do {                                                      \
    utils::Stringstream accumulator;                        \
    accumulator << message;                                 \
    logger::Logger::PushLog(                                \
        logger_var,                                         \
        level,                                              \
        accumulator.str(),                                  \
        logger::LogLocation(__FILE__, __FUNCTION__, line)); \
  } while (false)

#define LOG_WITH_LEVEL_EXT(logger_var, level, message, location)             \
  do {                                                                       \
    utils::Stringstream accumulator;                                         \
    accumulator << message;                                                  \
    logger::Logger::PushLog(logger_var, level, accumulator.str(), location); \
  } while (false)

#define LOG_MESSAGE(log_level, message) \
  LOG_WITH_LEVEL(                       \
      logger_module_ptr, logger::LogLevel::log_level, message, __LINE__)

#define LOG_TRACE(message) LOG_MESSAGE(LL_TRACE, message)

#define LOG_DEBUG(message) LOG_MESSAGE(LL_DEBUG, message)

#define LOG_INFO(message) LOG_MESSAGE(LL_INFO, message)

#define LOG_WARN(message) LOG_MESSAGE(LL_WARN, message)

#define LOG_ERROR(message) LOG_MESSAGE(LL_ERROR, message)

#define LOG_FATAL(message) LOG_MESSAGE(LL_FATAL, message)

namespace logger {
class AutoTrace {
 public:
  AutoTrace(const LoggerType& logger, const LogLocation& location)
      : logger_(logger), location_(location) {
    LOG_WITH_LEVEL_EXT(logger_, LogLevel::LL_TRACE, "Enter", location_);
  }

  ~AutoTrace() {
    LOG_WITH_LEVEL_EXT(logger_, LogLevel::LL_TRACE, "Exit", location_);
  }

 private:
  const LoggerType& logger_;
  const LogLocation location_;
};
}  // namespace logger

#define LOG_AUTO_TRACE()        \
  logger::AutoTrace auto_trace( \
      logger_module_ptr,        \
      logger::LogLocation(__FILE__, __FUNCTION__, __LINE__));
#else
#define CREATE_LOGGER(logger_name)
#define LOG_WITH_LEVEL(logger_var, level, message, line)
#define LOG_WITH_LEVEL_EXT(logger_var, level, message, location)
#define LOG_MESSAGE(log_level, message)
#define LOG_TRACE(message)
#define LOG_DEBUG(message)
#define LOG_INFO(message)
#define LOG_WARN(message)
#define LOG_ERROR(message)
#define LOG_FATAL(message)
#define LOG_WARN_WITH_ERRNO(message)
#define LOG_ERROR_WITH_ERRNO(message)
#define LOG_AUTO_TRACE()
#endif

#include "utils/date_time.h"

#include <chrono>

namespace utils {
namespace date_time {
std::chrono::system_clock::time_point GetTimeStamp() {
  return std::chrono::system_clock::now();
}

String GetDateTimeString(const String& format, const std::chrono::system_clock::time_point& chrono_time) {
  std::time_t time = std::chrono::system_clock::to_time_t(chrono_time);

  char buffer[80];
  std::strftime(buffer, sizeof(buffer), format.c_str(), std::localtime(&time));

  return String(buffer) + "," + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(chrono_time.time_since_epoch()).count() % 1000);
}

String GetDateTimeString(const String &format)
{
  return GetDateTimeString(format, GetTimeStamp());
}

}  // namespace date_time
}  // namespace utils

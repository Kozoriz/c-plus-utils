#include "utils/date_time.h"

#include <ctime>

namespace utils {
namespace date_time {
UInt32 GetTimeStamp() {
  return std::time(nullptr);
}

String GetDateTimeString(const String& format) {
  time_t rawtime;
  struct tm* timeinfo;
  char buffer[80];

  std::time(&rawtime);
  timeinfo = std::localtime(&rawtime);

  std::strftime(buffer, sizeof(buffer), format.c_str(), timeinfo);
  return String(buffer);
}

}  // namespace date_time
}  // namespace utils

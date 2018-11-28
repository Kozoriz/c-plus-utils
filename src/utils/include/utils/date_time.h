#pragma once

#include "utils/numbers.h"
#include "utils/containers/string.h"

#include <chrono>

namespace utils {
namespace date_time {

std::chrono::system_clock::time_point GetTimeStamp();
/**
 * @brief GetDateTimeString
 * @param format - comparable with strftime(...) "%d-%m-%Y %I:%M:%S"
 * @return formated time
 */
String GetDateTimeString(const String& format, const std::chrono::system_clock::time_point &time);

// returns time now
String GetDateTimeString(const String& format);

}  // namespace date_time
}  // namespace utils

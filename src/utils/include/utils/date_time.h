#pragma once

#include "utils/numbers.h"
#include "utils/containers/string.h"

namespace utils {
namespace date_time {

UInt32 GetTimeStamp();
/**
 * @brief GetDateTimeString
 * @param format - comparable with strftime(...) "%d-%m-%Y %I:%M:%S"
 * @return formated time
 */
String GetDateTimeString(const String& format);

}  // namespace date_time
}  // namespace utils

#pragma once

#include <boost/lexical_cast.hpp>
#include <sstream>
#include <string>

namespace utils {
/**
 * @brief String - TODO implement own string
 */
typedef std::string String;
typedef std::stringstream Stringstream;

template <typename Type>
utils::String ConvertToString(Type value) {
  return std::to_string(value);
}
}

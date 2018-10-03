#pragma once

#include "utils/containers/string.h"
#include "utils/structures/position.h"
#include "utils/containers/vector.h"

namespace utils
{
  Vector<String> SplitCSV(const String& str, const String& delim);

namespace  {
  template <typename Type>
  String ConvertToString(Type value) {
    return std::to_string(value);
  }

  template<typename T>
  T convertTo(const String& str)
  {
    return 0;
  }

  template<>
  Double convertTo(const String& str)
  {
    return std::stod(str);
  }

  template<>
  Int convertTo(const String& str)
  {
    return std::stoi(str);
  }

  template<>
  Float convertTo(const String& str)
  {
    return std::stof(str);
  }

  template<>
  positions::Location3 convertTo(const String& str)
  {
    positions::Location3 point;
    Vector<String> splited = SplitCSV(str, " ");
    point.x_ = convertTo<Int>(splited[0]);
    point.y_ = convertTo<Int>(splited[1]);
    point.z_ = convertTo<Int>(splited[2]);

    return point;
  }

} // namespace
} // namespace utils

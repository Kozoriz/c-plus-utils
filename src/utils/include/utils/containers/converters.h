#pragma once

#include "utils/containers/string.h"
#include "utils/structures/position.h"
#include "utils/containers/vector.h"

#ifdef PCL_INCLUDED
#include <pcl/point_types.h>
#endif

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
    Vector<String> splited = SplitCSV(str, ",");
    if(3 > splited.size()) return point;
    point.x_ = convertTo<Int>(splited[0]);
    point.y_ = convertTo<Int>(splited[1]);
    point.z_ = convertTo<Int>(splited[2]);

    return point;
  }

#ifdef PCL_INCLUDED
  template<>
  String ConvertToString(pcl::PointXYZRGB& point)
  {
    utils::Stringstream ss;
    ss << point.x << " " <<
          point.y << " " <<
          point.z << " " <<
          point.rgb;
    return ss.str();
  }

  template<>
  pcl::PointXYZRGB convertTo(const String& str)
  {
    pcl::PointXYZRGB point(128,128,128);
    Vector<String> splited = SplitCSV(str, ",");
    if(3 > splited.size()) return point;
    point.x = convertTo<Float>(splited[0]);
    point.y = convertTo<Float>(splited[1]);
    point.z = convertTo<Float>(splited[2]);
    if(3 < splited.size())
    {
      point.rgb = convertTo<Float>(splited[3]);
    }
    return point;
  }
#endif

} // namespace
} // namespace utils

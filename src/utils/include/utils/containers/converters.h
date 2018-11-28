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
    return std::stof(str.c_str());
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
  String ConvertToString(pcl::PointXYZRGBL& point)
  {
    utils::Stringstream ss;
    ss << point.x << " " <<
          point.y << " " <<
          point.z << " " <<
          point.rgb << " " <<
          point.label;
    return ss.str();
  }
  template<>
  pcl::PointXYZRGBL convertTo(const String& str)
  {
    pcl::PointXYZRGBL point(128,128,128,-1);
    Vector<String> splited = SplitCSV(str, ",");
    if(3 > splited.size()) return point;

    std::string& xs = splited[0];
//    std::replace(xs.begin(), xs.end(), '.', ',');
    std::string& ys = splited[1];
//    std::replace(ys.begin(), ys.end(), '.', ',');
    std::string& zs = splited[2];
//    std::replace(zs.begin(), zs.end(), '.', ',');

    point.x = convertTo<Float>(xs);
    point.y = convertTo<Float>(ys);
    point.z = convertTo<Float>(zs);
    if(3 < splited.size())
    {
      point.rgb = convertTo<Float>(splited[3]);
    }
    if(4 < splited.size())
    {
      point.label = convertTo<uint32_t>(splited[4]);
    }
    return point;
  }
#endif

} // namespace
} // namespace utils

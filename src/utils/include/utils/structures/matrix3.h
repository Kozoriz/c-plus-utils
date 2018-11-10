#pragma once

#include <functional>

#include "utils/containers/set.h"
#include "utils/structures/position.h"
#include "utils/containers/string.h"

namespace std {
template<>
class hash<utils::positions::Location3>{
public:
  size_t operator()(const utils::positions::Location3& b) const
  {
    return hash<utils::String>()(b.ToString());
  }
};
} // namespace std

namespace utils {
namespace structures {

class Matrix3 : public UnorderedSet<positions::Location3> {
 public:
  typedef UnorderedSet<positions::Location3> ContainerType;

  bool IsExists(const positions::Location3& pos) const;
  bool IsExists(const UInt x, const UInt y, const UInt z) const;

  void AddPoint(const positions::Location3& pos);
  void AddPoint(const UInt x, const UInt y, const UInt z);

  void RemovePoint(const positions::Location3& pos);
  void RemovePoint(const UInt x, const UInt y, const UInt z);

  Matrix3& operator+=(const Matrix3& another_matrix);
  Matrix3 operator+(const Matrix3& another_matrix);
};

}  // namepace structures
}  // namespace utils

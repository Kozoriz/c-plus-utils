#pragma once

#include "utils/containers/set.h"
#include "utils/structures/position.h"

namespace utils {
namespace structures {

class Hash {
 public:
  size_t operator()(const positions::Location3& pos) const;
};

class Matrix3 {
 public:
  typedef UnorderedSet<positions::Location3, Hash> ContainerType;

  bool IsExists(const positions::Location3& pos) const;
  bool IsExists(const UInt x, const UInt y, const UInt z) const;

  void AddPoint(const positions::Location3& pos);
  void AddPoint(const UInt x, const UInt y, const UInt z);

  void RemovePoint(const positions::Location3& pos);
  void RemovePoint(const UInt x, const UInt y, const UInt z);

  Matrix3& operator+=(const Matrix3& another_matrix);
  Matrix3 operator+(const Matrix3& another_matrix);

  UInt Size() const;
  void Clear();

  ContainerType::const_iterator begin() const;
  ContainerType::const_iterator end() const;

 private:
  ContainerType storage_;
};

}  // namepace structures
}  // namespace utils

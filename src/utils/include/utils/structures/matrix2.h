#pragma once

#include "utils/containers/set.h"
#include "utils/structures/position.h"

namespace utils {
namespace structures {
class Hash {
 public:
  size_t operator()(const positions::Location2& pos) const;
};

/**
 * @brief The Matrix2 - saves values of x,y coordinates in horizontal projection
 */
class Matrix2 {
 public:
  typedef UnorderedSet<positions::Location2, Hash> ContainerType;

  bool IsExists(const positions::Location2& pos) const;
  bool IsExists(const UInt x, const UInt y) const;

  void AddPoint(const positions::Location2& pos);
  void AddPoint(const UInt x, const UInt y);

  void RemovePoint(const positions::Location2& pos);
  void RemovePoint(const UInt x, const UInt y);

  const Matrix2& operator+=(const Matrix2& another_matrix);

  utils::UInt Size() const;

 private:
  ContainerType storage_;
};

}  // namespace structures
}  // namespace utils

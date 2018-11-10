#include "utils/structures/matrix3.h"
#include "utils/containers/string.h"

namespace utils {
namespace structures {

//size_t Hash::operator()(const positions::Location3& pos) const {
//  Stringstream ss;
//  ss << "[" << pos.x_ << ", " << pos.y_ << ", " << pos.z_ << "]";
//  String value = ss.str();
//  return HashFunc<String>()(value);
//}

bool Matrix3::IsExists(const positions::Location3& pos) const {
  return end() != find(pos);
}

bool Matrix3::IsExists(const UInt x, const UInt y, const UInt z) const {
  positions::Location3 pos;
  pos.x_ = x;
  pos.y_ = y;
  pos.z_ = z;
  return IsExists(pos);
}

void Matrix3::AddPoint(const positions::Location3& pos) {
  insert(pos);
}

void Matrix3::AddPoint(const UInt x, const UInt y, const UInt z) {
  positions::Location3 pos;
  pos.x_ = x;
  pos.y_ = y;
  pos.z_ = z;
  AddPoint(pos);
}

void Matrix3::RemovePoint(const positions::Location3& pos) {
  erase(pos);
}

void Matrix3::RemovePoint(const UInt x, const UInt y, const UInt z) {
  positions::Location3 pos;
  pos.x_ = x;
  pos.y_ = y;
  pos.z_ = z;
  RemovePoint(pos);
}

Matrix3& Matrix3::operator+=(const Matrix3& another_matrix) {
  insert(another_matrix.begin(),
         another_matrix.end());
  return *this;
}

Matrix3 Matrix3::operator+(const Matrix3& another_matrix) {
  Matrix3 temp;
  for (auto point : *this) {
    temp.AddPoint(point);
  }
  for (auto point : another_matrix) {
    temp.AddPoint(point);
  }
  return temp;
}

}  // namespace structures
}  // namepsace utils

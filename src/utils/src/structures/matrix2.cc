#include "utils/structures/matrix2.h"
#include "utils/containers/string.h"

namespace utils {
namespace structures {

size_t Hash::operator()(const positions::Location2& pos) const {
  Stringstream ss;
  ss << "[" << pos.x_ << ", " << pos.y_ << "]";
  String value = ss.str();
  return HashFunc<String>()(value);
}

bool Matrix2::IsExists(const positions::Location2& pos) const {
  return storage_.find(pos) != storage_.end();
}

bool Matrix2::IsExists(const UInt x, const UInt y) const {
  positions::Location2 pos;
  pos.x_ = x;
  pos.y_ = y;
  return IsExists(pos);
}

void Matrix2::AddPoint(const positions::Location2& pos) {
  storage_.insert(pos);
}

void Matrix2::AddPoint(const UInt x, const UInt y) {
  positions::Location2 pos;
  pos.x_ = x;
  pos.y_ = y;
  AddPoint(pos);
}

void Matrix2::RemovePoint(const positions::Location2& pos) {
  storage_.erase(pos);
}

void Matrix2::RemovePoint(const UInt x, const UInt y) {
  positions::Location2 pos;
  pos.x_ = x;
  pos.y_ = y;
  RemovePoint(pos);
}

const Matrix2& Matrix2::operator+=(const Matrix2& another_matrix) {
  storage_.insert(another_matrix.storage_.begin(),
                  another_matrix.storage_.end());
  return *this;
}

UInt Matrix2::Size() const {
  return storage_.size();
}

}  // namespace structures
}  // namespace utils

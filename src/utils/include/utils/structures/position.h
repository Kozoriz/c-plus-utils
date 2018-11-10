#pragma once

#include "utils/numbers.h"
#include "utils/containers/string.h"

namespace utils {
namespace positions {
/**
 * @brief The Incline struct - angles cant be more than 359 = 9bits
 */
struct Incline {
  Int16 alpha_ = 0;
  Int16 beta_ = 0;
};

struct Location2 {
  Int x_ = 0;
  Int y_ = 0;

  bool operator==(const Location2& other) const {
    return other.x_ == x_ && other.y_ == y_;
  }
};

class Location3 {
public:
  Int x_;
  Int y_;
  Int z_;

  Location3() : x_(0), y_(0), z_(0){}

  Location3(const Location3& other)
  {
    x_ = other.x_;
    y_ = other.y_;
    z_ = other.z_;
  }

  Location3(int x, int y, int z) : x_(x), y_(y), z_(z) {}

  bool operator==(const Location3& other) const {
    return other.x_ == x_ && other.y_ == y_ && other.z_ == z_;
  }

  bool operator!=(const Location3& other) const {
    return !(*this == other);
  }
  Location3 operator+(const Location3& other) const {
    Location3 temp;
    temp.x_ = this->x_ + other.x_;
    temp.y_ = this->y_ + other.y_;
    temp.z_ = this->z_ + other.z_;
    return temp;
  }

  String ToString() const {
    Stringstream ss;
    ss << x_ << " " << y_ << " " << z_;
    return ss.str();
  }
};

}  // positions
}  // utils

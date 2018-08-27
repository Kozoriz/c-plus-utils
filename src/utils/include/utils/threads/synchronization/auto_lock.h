#pragma once

#include "utils/threads/synchronization/lock.h"

namespace utils {
namespace synchronization {
class AutoLock {
 public:
  AutoLock(Lock& lock);
  ~AutoLock();

 private:
  Lock& lock_;
};
}
}

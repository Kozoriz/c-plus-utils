#pragma once

#include "utils/numbers.h"
#include "utils/threads/synchronization/atomic.h"
#include "utils/threads/synchronization/conditional_variable.h"
#include "utils/threads/synchronization/lock.h"

namespace utils {
namespace synchronization {
class Barrier {
 public:
  Barrier(const Int count);
  void set_count(const Int count);
  void Wait();

 private:
  Atomic<Int> count_;
  Lock lock_;
  ConditionalVariable cond_var_;
};
}
}

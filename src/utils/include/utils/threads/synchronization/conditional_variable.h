#pragma once

#include <condition_variable>
#include "utils/numbers.h"
#include "utils/threads/synchronization/lock.h"

namespace utils {
namespace synchronization {
class ConditionalVariable {
 public:
  enum class ExitState { kNoTimeout, kTimeout };

  void Wait(Lock& lock);
  ExitState WaitFor(Lock& lock, const UInt milliseconds);
  void NotifyOne();
  void Broadcast();

 private:
  std::condition_variable std_cond_var_;
};
}
}

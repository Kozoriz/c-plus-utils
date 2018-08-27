#pragma once
#include "utils/numbers.h"
#include "utils/threads/synchronization/auto_lock.h"
#include "utils/threads/synchronization/conditional_variable.h"
#include "utils/threads/synchronization/lock.h"

namespace utils {

class TestAsyncWaiter {
 public:
  TestAsyncWaiter() : notified_(false), count_(0), lock_(), cond_var_() {}

  bool WaitFor(const uint32_t wait_count, const uint32_t milliseconds) {
    while (count_ < wait_count) {
      notified_ = false;
      if (utils::synchronization::ConditionalVariable::ExitState::kTimeout ==
          cond_var_.WaitFor(lock_, milliseconds)) {
        return false;
      }
    }
    return true;
  }

  void Notify() {
    notified_ = true;
    ++count_;
    cond_var_.Broadcast();
  }

 private:
  bool notified_;
  utils::UInt count_;
  utils::synchronization::Lock lock_;
  utils::synchronization::ConditionalVariable cond_var_;
};

ACTION_P(NotifyTestAsyncWaiter, test_async_waiter) {
  test_async_waiter->Notify();
}

}  // namespace utils

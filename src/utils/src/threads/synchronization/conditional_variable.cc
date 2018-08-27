#include "utils/threads/synchronization/conditional_variable.h"

namespace utils {
namespace synchronization {

void ConditionalVariable::Wait(Lock& lock) {
  std::unique_lock<std::mutex> std_unq_lock(lock.std_mutex_);
  std_cond_var_.wait(std_unq_lock);
}

ConditionalVariable::ExitState ConditionalVariable::WaitFor(
    Lock& lock, const utils::UInt milliseconds) {
  std::unique_lock<std::mutex> std_unq_lock(lock.std_mutex_);
  if (std::cv_status::no_timeout ==
      std_cond_var_.wait_for(std_unq_lock,
                             std::chrono::milliseconds(milliseconds))) {
    return ExitState::kNoTimeout;
  } else {
    return ExitState::kTimeout;
  }
}

void ConditionalVariable::NotifyOne() {
  std_cond_var_.notify_one();
}

void ConditionalVariable::Broadcast() {
  std_cond_var_.notify_all();
}
}
}

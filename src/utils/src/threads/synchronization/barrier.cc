#include "utils/threads/synchronization/barrier.h"

namespace utils {
namespace synchronization {

Barrier::Barrier(const Int count) : count_(count) {}

void Barrier::set_count(const Int count) {
  count_ = count;
}

void Barrier::Wait() {
  if (--count_ <= 0) {
    cond_var_.Broadcast();
  } else {
    cond_var_.Wait(lock_);
  }
}

}  // namespace sychronizatiion
}  // namespace utils

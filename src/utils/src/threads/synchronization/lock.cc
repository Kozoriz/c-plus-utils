#include "utils/threads/synchronization/lock.h"

utils::synchronization::Lock::Lock() {}

void utils::synchronization::Lock::Acquire() {
  std_mutex_.lock();
}
void utils::synchronization::Lock::Release() {
  std_mutex_.unlock();
}

void utils::synchronization::Lock::Try() {
  std_mutex_.try_lock();
}

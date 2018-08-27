#include "utils/threads/synchronization/auto_lock.h"

utils::synchronization::AutoLock::AutoLock(Lock& lock) : lock_(lock) {
  lock_.Acquire();
}

utils::synchronization::AutoLock::~AutoLock() {
  lock_.Release();
}

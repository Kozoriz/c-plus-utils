#pragma once

#include <mutex>
#include "utils/macro.h"

namespace utils {
namespace synchronization {
class Lock {
 public:
  Lock();
  void Acquire();
  void Release();
  void Try();

 private:
  std::mutex std_mutex_;
  friend class ConditionalVariable;
  DISALLOW_COPY_AND_ASSIGN(Lock)
};
}
}

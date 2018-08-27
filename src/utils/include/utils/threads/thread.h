#pragma once

#include <thread>

#include "utils/pointers/unique_ptr.h"
#include "utils/threads/thread_runnable.h"

namespace utils {
namespace threads {
class Thread {
 public:
  enum class ThreadState { Stopped, Runnung };

  Thread(ThreadRunnable& runnable);
  bool StartThread();
  void JoinThread();
  void ThreadMain();

 private:
  ThreadRunnable& runnable_;
  utils::UniquePtr<std::thread> std_thread_unique_ptr_;
  ThreadState current_state_;
};
}
}

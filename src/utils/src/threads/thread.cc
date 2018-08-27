#include "utils/threads/thread.h"

utils::threads::Thread::Thread(utils::threads::ThreadRunnable& runnable)
    : runnable_(runnable), current_state_(ThreadState::Stopped) {}

bool utils::threads::Thread::StartThread() {
  std_thread_unique_ptr_ =
      utils::make_unique<std::thread>(&Thread::ThreadMain, this);
  current_state_ = ThreadState::Runnung;
  return true;
}

void utils::threads::Thread::JoinThread() {
  if (ThreadState::Runnung == current_state_ &&
      std_thread_unique_ptr_->joinable()) {
    runnable_.Join();
    std_thread_unique_ptr_->join();
  }
  std_thread_unique_ptr_.reset();
  current_state_ = ThreadState::Stopped;
}

void utils::threads::Thread::ThreadMain() {
  runnable_.Run();
}

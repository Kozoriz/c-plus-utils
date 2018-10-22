#include "utils/threads/thread.h"
#include "utils/logger.h"

CREATE_LOGGER("Utils")

utils::threads::Thread::Thread(utils::threads::ThreadRunnable& runnable)
    : runnable_(runnable), current_state_(ThreadState::Stopped) {}

bool utils::threads::Thread::StartThread() {
  LOG_AUTO_TRACE();
  std_thread_unique_ptr_ =
      utils::make_unique<std::thread>(&Thread::ThreadMain, this);
  current_state_ = ThreadState::Runnung;
  return true;
}

void utils::threads::Thread::JoinThread() {
    LOG_AUTO_TRACE();
  if (ThreadState::Runnung == current_state_ &&
      std_thread_unique_ptr_->joinable()) {
    runnable_.Join();
    std_thread_unique_ptr_->join();
  }
  std_thread_unique_ptr_.reset();
  current_state_ = ThreadState::Stopped;
}

void utils::threads::Thread::ThreadMain() {
  LOG_AUTO_TRACE();
  runnable_.Run();
}

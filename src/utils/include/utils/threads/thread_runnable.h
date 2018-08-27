#pragma once

namespace utils {
namespace threads {
class ThreadRunnable {
 public:
  virtual void Run() = 0;
  virtual void Join() = 0;
};
}
}

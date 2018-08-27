#pragma once

namespace utils {
template <typename MessageType>
class Observer {
 public:
  virtual void Handle(const MessageType& message) = 0;
};
}

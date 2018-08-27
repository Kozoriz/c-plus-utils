#pragma once

#include "utils/network/socket.h"

namespace utils {
class SocketServer : public Socket {
 public:
  virtual void AcceptClient() = 0;
};
}  // namespace utils

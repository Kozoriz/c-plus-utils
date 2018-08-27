#pragma once
#include "gmock/gmock.h"
#include "utils/network/socket.h"

namespace utils {

class MockSocket : public SocketClient {
 public:
  MOCK_METHOD1(Send, void(const ByteArray& message));
  MOCK_METHOD0(Receive, utils::ByteArray());
  MOCK_METHOD0(Init, void());
};

}  // namespace utils

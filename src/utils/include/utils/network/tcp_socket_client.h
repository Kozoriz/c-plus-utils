#pragma once

#include "utils/containers/string.h"
#include "utils/network/socket_client.h"
#include "utils/containers/message_queue.h"
#include "utils/pointers/unique_ptr.h"

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

namespace utils {
typedef MessageQueue<ByteArray> ClientMessageQueue;

class TcpSocketClient : public SocketClient {
 public:
  TcpSocketClient(const utils::String& address, const UInt port);
  ~TcpSocketClient();
  void Send(const ByteArray& message) override;
  ByteArray Receive() override;
  void Init() override;
  void Deinit();

 private:
  void EstabilishConnection();
  void OnConnetionEstablished(const boost::system::error_code& error);
  void OnMessageReceived(const boost::system::error_code& error);
  void SocketRead(const boost::system::error_code& error);
  void WritingTask(ByteArray message);
  void SocketWrite(const boost::system::error_code& error);

 private:
  const utils::String address_;
  const UInt port_;
  ByteArray buffer_;
  ClientMessageQueue messages_to_server_;
  ClientMessageQueue messages_from_server_;

  boost::asio::io_service boost_io_service_;
  boost::asio::ip::tcp::socket boost_socket_;
  UniquePtr<boost::thread> boost_thread_ptr_;
};
}  // namespace utils

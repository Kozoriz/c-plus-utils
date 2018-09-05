#pragma once

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include "utils/network/socket_server.h"

#include "utils/containers/map.h"
#include "utils/pointers/shared_prt.h"
#include "utils/pointers/unique_ptr.h"
#include "utils/threads/synchronization/atomic.h"
#include "utils/containers/message_queue.h"

namespace utils {

typedef boost::asio::ip::tcp::socket BoostSocket;
typedef SharedPtr<BoostSocket> BoostSocketSPtr;
typedef MessageQueue<ByteArray> ServerMessageQueue;

class TcpSocketServer : public SocketServer {
 public:
  TcpSocketServer(const UInt16 port);
  ~TcpSocketServer();
  void Send(const ByteArray& message) override;
  ByteArray Receive() override;
  void AcceptClient() override;
  void Init() override;

 private:
  void OnClientAccepted(const boost::system::error_code& error);

  class ClientSession {
   public:
    ClientSession(BoostSocketSPtr socket);
    void WriteDataToSocket(const ByteArray& message);
    const ByteArray ReadDataFromSocket();
    void AllowTransmmitting(const bool is_accepted);

   private:
    void SocketWriteTask(const boost::system::error_code& error);
    void SocketReadTask(const boost::system::error_code& error);

   private:
    utils::SharedPtr<BoostSocket> socket_sptr_;
    ByteArray buffer_;
    ServerMessageQueue messages_to_client_;
    ServerMessageQueue messages_from_client_;

    synchronization::AtomicBool read_write_allowed_;
    synchronization::AtomicBool reading_in_progress_;
  };

 private:
  const UInt16 port_;
  SharedPtr<ClientSession> client_;

  boost::asio::io_service boost_io_service_;
  boost::asio::ip::tcp::acceptor boost_acceptor_;
  UniquePtr<boost::thread> boost_thread_ptr_;
};

}  // namespace utils

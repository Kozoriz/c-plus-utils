
#include <boost/asio.hpp>

#include "utils/network/tcp_socket_client.h"
#include "utils/threads/synchronization/conditional_variable.h"

#include "utils/logger.h"

CREATE_LOGGER("Utils")

namespace utils {
using namespace boost::asio::ip;

namespace {
// TODO use profile
const UInt max_buffer_size = 5u;
// TODO use profile
const UInt connect_waiting_timeout_ms = 5000u;
}

TcpSocketClient::TcpSocketClient(const utils::String& address, const UInt port)
    : address_(address), port_(port), boost_socket_(boost_io_service_) {
  LOG_AUTO_TRACE();
  buffer_.resize(max_buffer_size);
}

TcpSocketClient::~TcpSocketClient() {
  LOG_AUTO_TRACE();
  Deinit();
}

void TcpSocketClient::Init() {
  LOG_AUTO_TRACE();
  EstabilishConnection();
}

void TcpSocketClient::Deinit() {
  LOG_AUTO_TRACE();
  boost_socket_.close();
  boost_thread_ptr_->yield();
  boost_thread_ptr_.reset();
}

void TcpSocketClient::Send(const ByteArray& message) {
  LOG_AUTO_TRACE();
  boost_io_service_.post(
      boost::bind(&TcpSocketClient::WritingTask, this, message));
}

ByteArray TcpSocketClient::Receive() {
  LOG_AUTO_TRACE();
  return messages_from_server_.GetMessage();
}

void TcpSocketClient::EstabilishConnection() {
  LOG_AUTO_TRACE();
  const utils::String str_port = utils::ConvertToString(port_);
  tcp::resolver resolver(boost_io_service_);
  tcp::resolver::query resolver_query(address_, str_port);
  boost::system::error_code ec;
  tcp::resolver::iterator resolver_iterator;

  synchronization::Lock wait_lock;
  utils::synchronization::ConditionalVariable wait_cv;
  while (true) {
    LOG_DEBUG("Trying to resolve : " << address_ << ":" << str_port);
    resolver_iterator = resolver.resolve(resolver_query, ec);
    if (tcp::resolver::iterator() != resolver_iterator) {
      boost::asio::async_connect(
          boost_socket_,
          resolver_iterator,
          boost::bind(&TcpSocketClient::OnConnetionEstablished,
                      this,
                      boost::asio::placeholders::error));
      boost_thread_ptr_ = make_unique<boost::thread>(
          boost::bind(&boost::asio::io_service::run, &boost_io_service_));
      break;
    }
    LOG_DEBUG("Resolving failed : " << ec.message() << ". Wait for : "
                                    << connect_waiting_timeout_ms);
    wait_cv.WaitFor(wait_lock, connect_waiting_timeout_ms);
  }
}

void TcpSocketClient::OnConnetionEstablished(
    const boost::system::error_code& error) {
  LOG_AUTO_TRACE();
  LOG_DEBUG("Client socket connected : " << address_ << ":" << port_);
  SocketRead(error);
}

void TcpSocketClient::OnMessageReceived(
    const boost::system::error_code& error) {
  LOG_AUTO_TRACE();
  if (!buffer_.empty()) {
    LOG_DEBUG("Message received!");
    messages_from_server_.PushMessage(buffer_);
    buffer_.clear();
    buffer_.resize(max_buffer_size);
  }
  SocketRead(error);
}

void TcpSocketClient::SocketRead(const boost::system::error_code& error) {
  LOG_AUTO_TRACE();
  if (!error) {
    boost::asio::async_read(boost_socket_,
                            boost::asio::buffer(buffer_, max_buffer_size),
                            boost::bind(&TcpSocketClient::OnMessageReceived,
                                        this,
                                        boost::asio::placeholders::error));
    return;
  }
  Deinit();
}

void TcpSocketClient::WritingTask(ByteArray message) {
  LOG_AUTO_TRACE();
  bool is_writing_in_progress = !messages_to_server_.IsEmpty();
  messages_to_server_.PushMessage(message);
  if (!is_writing_in_progress) {
    SocketWrite(boost::system::error_code());
  }
}

void TcpSocketClient::SocketWrite(const boost::system::error_code& error) {
  LOG_AUTO_TRACE();
  if (!error) {
    if (!messages_to_server_.IsEmpty()) {
      boost::asio::async_write(
          boost_socket_,
          boost::asio::buffer(messages_to_server_.GetMessage(),
                              max_buffer_size),
          boost::bind(&TcpSocketClient::SocketWrite,
                      this,
                      boost::asio::placeholders::error));
    }
    return;
  }
  Deinit();
}
}

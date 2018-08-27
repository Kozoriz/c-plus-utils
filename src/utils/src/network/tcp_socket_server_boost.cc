
#include <boost/asio/basic_socket.hpp>

#include "utils/network/tcp_socket_server.h"

#include "utils/logger.h"

CREATE_LOGGER("Utils")
#include <iomanip>

namespace utils {

namespace {
// TODO use profile
const utils::UInt max_buffer_size = 5u;
// TODO use profile
const utils::UInt connect_waiting_timeout_ms = 5000u;
}  // namespace

using namespace boost::asio::ip;

TcpSocketServer::TcpSocketServer(const UInt16 port)
    : port_(port)
    , boost_acceptor_(boost_io_service_, tcp::endpoint(tcp::v4(), port_)) {
  LOG_AUTO_TRACE();
}

TcpSocketServer::~TcpSocketServer() {
  LOG_AUTO_TRACE();
  client_->AllowTransmmitting(false);
  boost_thread_ptr_->join();
  boost_thread_ptr_.reset();
}

void TcpSocketServer::Send(const ByteArray& message) {
  LOG_AUTO_TRACE();
  client_->WriteDataToSocket(message);
}

ByteArray TcpSocketServer::Receive() {
  LOG_AUTO_TRACE();
  return client_->ReadDataFromSocket();
}

void TcpSocketServer::AcceptClient() {
  LOG_AUTO_TRACE();
  BoostSocketSPtr socket_ptr =
      utils::make_shared<tcp::socket>(boost_io_service_);
  client_ = utils::make_shared<ClientSession>(socket_ptr);
  boost_acceptor_.async_accept(*socket_ptr,
                               boost::bind(&TcpSocketServer::OnClientAccepted,
                                           this,
                                           boost::asio::placeholders::error));

  boost_thread_ptr_ = make_unique<boost::thread>(
      boost::bind(&boost::asio::io_service::run, &boost_io_service_));
}

void TcpSocketServer::OnClientAccepted(const boost::system::error_code& error) {
  LOG_AUTO_TRACE();
  if (!error) {
    client_->AllowTransmmitting(true);
    return;
  }
  LOG_ERROR("Client accepting error : " << error.message());
}

void TcpSocketServer::Init() {
  LOG_AUTO_TRACE();
}

TcpSocketServer::ClientSession::ClientSession(BoostSocketSPtr socket)
    : socket_sptr_(socket)
    , read_write_allowed_(false)
    , reading_in_progress_(false) {
  LOG_AUTO_TRACE();
}

void TcpSocketServer::ClientSession::WriteDataToSocket(
    const ByteArray& message) {
  LOG_AUTO_TRACE();
  const bool is_writing_in_progress = !messages_to_client_.IsEmpty();
  messages_to_client_.PushMessage(message);
  if (!is_writing_in_progress) {
    SocketWriteTask(boost::system::error_code());
  }
}

const ByteArray TcpSocketServer::ClientSession::ReadDataFromSocket() {
  LOG_AUTO_TRACE();
  if (!reading_in_progress_ && read_write_allowed_) {
    SocketReadTask(boost::system::error_code());
  }
  return messages_from_client_.GetMessage();
}

void TcpSocketServer::ClientSession::AllowTransmmitting(
    const bool is_accepted) {
  LOG_AUTO_TRACE();
  read_write_allowed_ = is_accepted;
  SocketReadTask(boost::system::error_code());
  SocketWriteTask(boost::system::error_code());
}

void TcpSocketServer::ClientSession::SocketWriteTask(
    const boost::system::error_code& error) {
  LOG_AUTO_TRACE();
  if (!error) {
    if (read_write_allowed_ && !messages_to_client_.IsEmpty()) {
      LOG_INFO("Writing socket data started");
      boost::asio::async_write(
          *socket_sptr_,
          boost::asio::buffer(messages_to_client_.GetMessage(),
                              max_buffer_size),
          boost::bind(&TcpSocketServer::ClientSession::SocketWriteTask,
                      this,
                      boost::asio::placeholders::error));
    }
    return;
  }
  LOG_ERROR("Socket write error : " << error.message());
}

void TcpSocketServer::ClientSession::SocketReadTask(
    const boost::system::error_code& error) {
  LOG_AUTO_TRACE();
  if (!error) {
    if (!buffer_.empty()) {
      LOG_INFO("Message received.");
      messages_from_client_.PushMessage(buffer_);
      LOG_DEBUG("Messages from client size " << messages_from_client_.Size());
      buffer_.clear();
    }
    buffer_.resize(max_buffer_size);
    LOG_INFO("Reading socket data started");
    boost::asio::async_read(
        *socket_sptr_,
        boost::asio::buffer(buffer_, max_buffer_size),
        boost::bind(&TcpSocketServer::ClientSession::SocketReadTask,
                    this,
                    boost::asio::placeholders::error));
    reading_in_progress_ = true;
    return;
  }
  reading_in_progress_ = false;
  if (error == boost::asio::error::eof) {
    LOG_DEBUG("Socket eof received");
    return;
  }
  LOG_ERROR("Socket read error : " << error.message());
}
}  // namespace utils

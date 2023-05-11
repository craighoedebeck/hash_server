#pragma once

#include <boost/asio.hpp>
#include "HashRequestHandler.hpp"

/// Represents a single connection
class HashConnection
  : public std::enable_shared_from_this<HashConnection>
{
public:
  //Make non-copyable
  HashConnection(const HashConnection&) = delete;
  HashConnection& operator=(const HashConnection&) = delete;

  /// Construct a HashConnection with the given io_context.
  explicit HashConnection(boost::asio::io_context& io_context, HashRequestHandler& handler);
      
  /// Get the socket associated with the HashConnection.
  boost::asio::ip::tcp::socket& socket();

  /// Start the first asynchronous operation for the HashConnection.
  void start();

private:
  /// Handle completion of a read operation.
  void handle_read(const boost::system::error_code& e,
      std::size_t bytes_transferred);

  /// Handle completion of a write operation.
  void handle_write(const boost::system::error_code& e);

  void read_some();

  /// Socket for the HashConnection.
  boost::asio::ip::tcp::socket m_socket;

  /// The handler used to process the incoming request.
  HashRequestHandler& m_request_handler;

  /// Buffer for incoming data.
  std::array<char, 8192> m_buffer;

  /// The reply to be sent back to the client.
  std::string m_reply;
};

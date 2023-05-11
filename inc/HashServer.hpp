#pragma once

#include <string>
#include <vector>

#include <boost/asio.hpp>

#include "HashConnection.hpp"
#include "ServerContextPool.hpp"

/// The top-level class of the hash HashServer
class HashServer
{
public:
  /// Construct the HashServer to listen on the specified TCP address and port, and give hashes of strings
  explicit HashServer(const std::string& address, std::size_t port, std::size_t num_contexts);

  /// Run the HashServer's io_context loop.
  void run();

private:
  /// Initiate an asynchronous accept operation.
  void start_accept();

  /// Handle completion of an asynchronous accept operation.
  void handle_accept(const boost::system::error_code& e);

  /// Handle a request to stop the HashServer.
  void handle_stop();

  /// The pool of context objects used to perform asynchronous operations.
  ServerContextPool m_context_pool;

  /// The signal_set is used to register for process termination notifications.
  boost::asio::signal_set m_signals;

  /// Acceptor used to listen for incoming connections.
  boost::asio::ip::tcp::acceptor m_acceptor;

  /// The next connection to be accepted.
  std::shared_ptr<HashConnection> m_new_connection;
};

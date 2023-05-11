#pragma once

#include <boost/asio.hpp>
#include <list>
#include <vector>

/// A pool of io_context objects.
class ServerContextPool
{
public:
  ServerContextPool(const ServerContextPool&) = delete;
  ServerContextPool& operator=(const ServerContextPool&) = delete;

  /// Construct the context pool.
  explicit ServerContextPool(std::size_t pool_size);

  /// Run all context objects in the pool.
  void run();

  /// Stop all context objects in the pool.
  void stop();

  /// Get an context to use.
  boost::asio::io_context& get_context();

private:
  typedef boost::asio::executor_work_guard<boost::asio::io_context::executor_type> io_context_work;
    
  /// The pool of io_contexts.
  std::vector<std::shared_ptr<boost::asio::io_context>> m_contexts;

  /// The work that keeps the io_contexts running.
  std::list<io_context_work> m_work;

  /// The next context to use for a connection.
  std::vector<std::shared_ptr<boost::asio::io_context>>::iterator m_next_context;
};

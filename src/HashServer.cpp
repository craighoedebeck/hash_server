#include "HashServer.hpp"

HashServer::HashServer(const std::string& address, std::size_t port, std::size_t context_pool_size)
  :m_context_pool(context_pool_size)
  ,m_signals(m_context_pool.get_context())
  ,m_acceptor(m_context_pool.get_context())
  ,m_new_connection()
{
  m_signals.add(SIGINT);
  m_signals.add(SIGTERM);
  m_signals.add(SIGQUIT);
  m_signals.async_wait(std::bind(&HashServer::handle_stop, this));

  //Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
  boost::asio::ip::tcp::resolver resolver(m_acceptor.get_executor());
  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, std::to_string(port)).begin();
  m_acceptor.open(endpoint.protocol());
  m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  m_acceptor.bind(endpoint);
  m_acceptor.listen();

  start_accept();
}

void HashServer::run()
{
  m_context_pool.run();
}

void HashServer::start_accept()
{
  m_new_connection.reset(new HashConnection(m_context_pool.get_context()));
        
  m_acceptor.async_accept(
    m_new_connection->socket(),
    std::bind(&HashServer::handle_accept, this, std::placeholders::_1)
  );
}

void HashServer::handle_accept(const boost::system::error_code& e)
{
  if (!e)
  {
    m_new_connection->start();
  }

  start_accept();
}

void HashServer::handle_stop()
{
  m_context_pool.stop();
}

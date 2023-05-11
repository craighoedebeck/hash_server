#include "HashConnection.hpp"
#include "HashRequestHandler.hpp"
#include "HashAlgorithms.hpp"

#include <vector>

HashConnection::HashConnection(boost::asio::io_context& io_context, HashRequestHandler& handler)
  :m_socket(io_context)
  ,m_request_handler(handler)
  ,m_buffer()
  ,m_reply(MD5_DIGEST_LENGTH*2 + 1, '\n') //all but last newline will get overwritten by hash algo
{
}

boost::asio::ip::tcp::socket& HashConnection::socket()
{
  return m_socket;
}

void HashConnection::read_some()
{
  m_socket.async_read_some(
    boost::asio::buffer(m_buffer),
    std::bind(
      &HashConnection::handle_read,
      shared_from_this(),
      std::placeholders::_1,
      std::placeholders::_2
    )
  );
}


void HashConnection::start()
{
  read_some();
}

void HashConnection::handle_read(
  const boost::system::error_code& e,
  std::size_t bytes_transferred)
{
  if (!e)
  {
    auto end_of_string = std::find(m_buffer.begin(), m_buffer.end(), '\n');

    if (end_of_string != m_buffer.end())
    {
      
      MD5Hash::calculate(std::string(m_buffer.begin(), end_of_string), m_reply);
      
      boost::asio::async_write(
        m_socket,
        boost::asio::buffer(m_reply),
        std::bind(&HashConnection::handle_write, shared_from_this(), std::placeholders::_1)
      );
    }
    else
    {
      read_some();
    }
  }
  else 
  {
    boost::system::error_code ignored_ec;
    m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }
}

void HashConnection::handle_write(const boost::system::error_code& e)
{
  if (e)
  {
    boost::system::error_code ignored_ec;
    m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }
  else {
    read_some();
  }
}

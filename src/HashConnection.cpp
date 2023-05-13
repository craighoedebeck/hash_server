#include "HashConnection.hpp"

#include <vector>

HashConnection::HashConnection(boost::asio::io_context& io_context)
  :m_socket(io_context)
  ,m_buffer()
  ,m_parser()
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

void HashConnection::schedule_write()
{
  //write my response
  boost::asio::async_write(
    m_socket,
    boost::asio::buffer(m_parser.get_reply()),
    std::bind(&HashConnection::handle_write, shared_from_this(), std::placeholders::_1)
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
    m_parser.receive_data(m_buffer.begin(), m_buffer.begin() + bytes_transferred);

    //If we have a reply to send, send it, otherwise read more data
    if(m_parser.has_reply())
    {
      schedule_write();
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
  else
  {
    //We have successfully written at this point, so clear the reply flag
    m_parser.has_reply(false);

    //If we have leftovers, process those leftovers, and send if we have a reply
    if(m_parser.has_leftover())
    {
      m_parser.process_leftovers();
      if(m_parser.has_reply())
      {
        schedule_write();
        return;
      } 
    }
    //Read some more if we didn't write anything this time
    read_some();
  }
}


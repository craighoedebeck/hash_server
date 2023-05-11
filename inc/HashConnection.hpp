#pragma once

#include "MessageParser.hpp"

#include <boost/asio.hpp>

//A single remote connection
class HashConnection
  : public std::enable_shared_from_this<HashConnection>
{
  public:
    //Make non-copyable
    HashConnection(const HashConnection&) = delete;
    HashConnection& operator=(const HashConnection&) = delete;

    explicit HashConnection(boost::asio::io_context& io_context);
        
    //Get the socket associated with the HashConnection.
    boost::asio::ip::tcp::socket& socket();

    //Start the first asynchronous operation for the HashConnection.
    //Future operations are kicked off in the handlers
    void start();

  private:
    //Handle completion of a read operation.
    void handle_read(const boost::system::error_code& e,
        std::size_t bytes_transferred);

    //Handle completion of a write operation.
    void handle_write(const boost::system::error_code& e);

    void read_some();

    //Socket for the HashConnection.
    boost::asio::ip::tcp::socket m_socket;

    //Buffer for incoming data.
    MessageParser::BufferContainer m_buffer;

    //Parser for each connection.  Could pool these if we don't want to deallocate
    //on each connection close.  Go simple for now.
    MessageParser m_parser;
};

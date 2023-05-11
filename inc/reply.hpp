#pragma once

#include <string>
#include <boost/asio.hpp>

/// A reply to be sent to a client.
struct reply
{
  /// The content to be sent in the reply.
  std::string hash;

  /// Convert the reply into a vector of buffers. The buffers do not own the
  /// underlying memory blocks, therefore the reply object must remain valid and
  /// not be changed until the write operation has completed.
  std::vector<boost::asio::const_buffer> to_buffers();
};

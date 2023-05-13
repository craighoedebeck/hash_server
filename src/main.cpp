#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <thread>

#include "HashServer.hpp"

int main(int /*argc*/, char** /*argv*/)
{
  try
  {
    // Initialise the server.
    std::size_t num_threads = std::thread::hardware_concurrency();
    HashServer s("0.0.0.0", 6060, num_threads);

    // Run the server until stopped.
    s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

#include "ServerContextPool.hpp"

#include <stdexcept>
#include <thread>

ServerContextPool::ServerContextPool(std::size_t size)
  :m_contexts()
  ,m_work()
  ,m_next_context()
{
  if (size == 0)
  {
    throw std::runtime_error("ServerContextPool size is 0");
  }

  //Give all the io_contexts work to do so that their run() functions will not
  //exit until they are explicitly stopped.
  for (std::size_t i = 0; i < size; ++i)
  {
    std::shared_ptr<boost::asio::io_context> context(new boost::asio::io_context);
    m_contexts.emplace_back(context);
    m_work.emplace_back(boost::asio::make_work_guard(*context));
  }
  m_next_context = m_contexts.begin();
}

void ServerContextPool::run()
{
  //Create a pool of threads to run all of the io_contexts.
  std::vector<std::shared_ptr<std::thread>> threads;
  for(auto context : m_contexts)
  {
    for(auto context : m_contexts) 
    {
      std::shared_ptr<std::thread> worker_thread(
        new std::thread([&context]{
          context->run();
        })
      );
      threads.emplace_back(worker_thread);
    }
  }

  //Wait for all threads in the pool to exit.
  for(auto thread : threads)
  {
    thread->join();
  }
}

void ServerContextPool::stop()
{
  //Stop all io_contexts.
  for(auto context : m_contexts) {
    context->stop();
  }
}

boost::asio::io_context& ServerContextPool::get_context()
{
  //Use a round-robin scheme to choose the next io_context to use.
  boost::asio::io_context& io_context = **m_next_context;
  ++m_next_context;
  if (m_next_context == m_contexts.end()) 
  {
    m_next_context = m_contexts.begin();
  }
  return io_context;
}

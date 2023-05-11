#include "MessageParser.hpp"

MessageParser::MessageParser(
):m_hasher()
 ,m_leftover()
 ,m_reply(REPLY_LENGTH, '\n') //all but last newline will get overwritten by hash algo
 ,m_has_reply(false)
{
}

void MessageParser::receive_data(
  BufferContainer::const_iterator begin,
  BufferContainer::const_iterator end
)
{
  //Append everything to leftover
  m_leftover.append(begin, end);

  //find the first newline in leftover
  auto end_of_string = m_leftover.find('\n');

  //If there is a terminator string in the set of messages
  if (end_of_string != std::string::npos)
  {
    //Hash what we received so far
    m_hasher.append_bytes(m_leftover.substr(0, end_of_string));
    //Make sure we erase the null terminator from leftover
    m_leftover.erase(0, end_of_string+1);


    //get the hash result
    m_hasher.finalize(m_reply);

    //clear my received bytes
    m_hasher.init_hasher();
    m_has_reply = true;
  }
  else
  {
    m_hasher.append_bytes(m_leftover);
    m_leftover.clear();
  }
}

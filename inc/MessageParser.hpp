#pragma once

#include "HashAlgorithms.hpp"

class MessageParser
{
  public:
    typedef std::array<char, 8192> BufferContainer;

    MessageParser();

    void receive_data(BufferContainer::const_iterator begin, BufferContainer::const_iterator end);
    inline bool has_reply() const { return m_has_reply; }
    inline void has_reply(bool reply) {m_has_reply = reply;}
    inline bool has_leftover() const { return !m_leftover.empty(); }
    //Get the reply.  Return as a reference so we can wrap it in an asio buffer
    inline std::string& get_reply() { return m_reply; }

  private:
    //Reply length depends on the length of the hash output
    //Assuming twice the MD5 sum length (for converting to hex characters)
    //and an extra byte for the newline.  If we want to support differt hash types
    //this will need to change
    static const int REPLY_LENGTH = MD5_DIGEST_LENGTH * 2 + 1;

    MD5Hash m_hasher;

    //Leftover bytes from a read to add to received next time
    std::string m_leftover;

    //The reply to be sent back to the client.
    std::string m_reply;

    //Do we have a reply to send?
    bool m_has_reply;
};

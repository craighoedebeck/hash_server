#pragma once

#include <openssl/md5.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

class MD5Hash {
  public:
    MD5Hash(
    ):m_context()
     ,m_needs_finalization(false)
    {
      init_hasher();
    }

    ~MD5Hash()
    {
      //Unclear if this is needed.  Doesn't leak memory if this is not called.
      //Including this here in case other resources (file handle maybe?) are obtained.
      if(m_needs_finalization)
      {
        uint8_t dummy_data[MD5_DIGEST_LENGTH];
        MD5_Final(dummy_data, &m_context);
      }
    }

    void init_hasher()
    {
      MD5_Init(&m_context);
      m_needs_finalization = true;
    }

    void append_bytes(const std::string& received_bytes)
    {
      MD5_Update(&m_context, received_bytes.data(), received_bytes.size());
    }

    void finalize(std::string& output)
    {
      MD5_Final(reinterpret_cast<uint8_t*>(output.data()), &m_context);
      md5_to_hex(output);
      m_needs_finalization = false;
    }

  private:
    static void md5_to_hex(std::string& output) {
      //Do some inplace bit level math to convert the value to a hex
      //string.  Output string should be twice the size of what MD5
      //calculated, so start from the end so we don't overwrite anything
      for(int i = MD5_DIGEST_LENGTH - 1; i>=0;i--) {
        uint8_t current_char = output[i];
        output[i*2] = nibble_to_hex(current_char >> 4);
        output[i*2 + 1] = nibble_to_hex(current_char & 0xF);
      }
    }

    static inline uint8_t nibble_to_hex(uint8_t nibble) {
      return (nibble < 10) ? nibble + '0' : nibble - 10 + 'a';
    }

    MD5_CTX m_context;
    bool m_needs_finalization;
};

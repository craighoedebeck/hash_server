#pragma once

#include <openssl/md5.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

class MD5Hash {
  public:
    //In place MD5 hash transformation in order to optimize memory
    static void calculate(const std::string& input, std::string& output) {
      //Calculate MD5 into the output string
      MD5(
        reinterpret_cast<const uint8_t*>(input.data()),
        input.length(),
        reinterpret_cast<uint8_t*>(output.data())
      );

      //Do some inplace bit level math to convert the value to a hex
      //string.  Output string should be twice the size of what MD5
      //calculated, so start from the end so we don't overwrite anything
      for(int i = MD5_DIGEST_LENGTH - 1; i>=0;i--) {
        uint8_t current_char = output[i];
        output[i*2] = nibble_to_hex(current_char >> 4);
        output[i*2 + 1] = nibble_to_hex(current_char & 0xF);
      }
    }
  private:
    static inline uint8_t nibble_to_hex(uint8_t nibble) {
      return (nibble < 10) ? nibble + '0' : nibble - 10 + 'a';
    }
};

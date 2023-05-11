#include "HashAlgorithms.hpp"
#include "MessageParser.hpp"

#include <gtest/gtest.h>

std::string expectedHash = "6cd3556deb0da54bca060b4c39479839";

void add_data_to_parser(const std::string data, MessageParser& parser)
{
  MessageParser::BufferContainer container;
  for(int i=0;i<data.size();i++) {
    container[i] = data[i];
  }

  std::cout << "Last character = " << *(container.begin() + data.size() - 1) << std::endl;

  parser.receive_data(container.begin(), container.begin() + data.size());
}

TEST(MD5HashTest, HashesInputString) {
  std::string output(MD5_DIGEST_LENGTH*2, 0);

  MD5Hash hasher;
  hasher.init_hasher();
  hasher.append_bytes(std::string("Hello, "));
  hasher.append_bytes(std::string("world!"));
  hasher.finalize(output);

  EXPECT_EQ(output, expectedHash);
}

TEST(MessageParserTest, ParseInputString) {
  MessageParser parser;

  add_data_to_parser("Hello, ", parser);
  EXPECT_FALSE(parser.has_reply());
  EXPECT_FALSE(parser.has_leftover()); 

  add_data_to_parser("world!\nHello, world!\n", parser);
  EXPECT_TRUE(parser.has_reply());
  EXPECT_TRUE(parser.has_leftover());
  EXPECT_EQ(parser.get_reply(), expectedHash + "\n");

  add_data_to_parser("", parser);
  EXPECT_TRUE(parser.has_reply());
  EXPECT_FALSE(parser.has_leftover());
  EXPECT_EQ(parser.get_reply(), expectedHash + "\n");
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#include "HashAlgorithms.hpp"

#include <gtest/gtest.h>

TEST(MD5HashTest, HashesInputString) {
  std::string input = "Hello, world!";
  std::string output(MD5_DIGEST_LENGTH*2, 0);
  std::string expectedHash = "6cd3556deb0da54bca060b4c39479839";

  MD5Hash::calculate(input, output);

  EXPECT_EQ(output, expectedHash);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

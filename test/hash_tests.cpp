#include "HashAlgorithms.hpp"
#include "MessageParser.hpp"
#include "HashServer.hpp"

#include <gtest/gtest.h>

std::string expectedHash = "6cd3556deb0da54bca060b4c39479839";

void add_data_to_parser(const std::string data, MessageParser& parser)
{
  MessageParser::BufferContainer container;
  for(int i=0;i<data.size();i++) {
    container[i] = data[i];
  }

  parser.receive_data(container.begin(), container.begin() + data.size());
}

void start_hash_server(std::shared_ptr<HashServer> s)
{
  ASSERT_NO_THROW(s->run());
}

//Connect in syncronously, write our hash string, and check what we read back is correct
//Really should be done with a full async setup, but this is probably okay for a unit test
void connect_write_read_test()
{
  int sockfd;
  int portno = 6060;
  int n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  std::string send_string("Hello, world!\n");
  std::string receive_string(MD5_DIGEST_LENGTH*2+1, 0);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  ASSERT_TRUE(sockfd >= 0);

  server = gethostbyname("localhost");
  ASSERT_TRUE(server != NULL);

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
  serv_addr.sin_port = htons(portno);

  int connect_result = connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
  ASSERT_TRUE(connect_result >= 0);

  //Normally would want to make sure all bytes got written, but cutting corners for a simple unit
  //test example
  n = write(sockfd, send_string.data(), send_string.size());
  ASSERT_TRUE(n >=0);

  //Normally would want to epoll or select here, but cutting corners for a simple unit
  //test example
  n = read(sockfd, receive_string.data(), receive_string.size());
  ASSERT_TRUE(n >= 0);
  ASSERT_EQ(receive_string, expectedHash + '\n');

  close(sockfd);
}


//Does the hasher work?
TEST(MD5HashTest, HashesInputString)
{
  std::string output(MD5_DIGEST_LENGTH*2, 0);

  MD5Hash hasher;
  hasher.init_hasher();
  hasher.append_bytes(std::string("Hello, "));
  hasher.append_bytes(std::string("world!"));
  hasher.finalize(output);

  EXPECT_EQ(output, expectedHash);
}

//Does the parser work?
TEST(MessageParserTest, ParseInputString)
{
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

//Make sure the server can properly handle large number of current connections
TEST(HashServerTest, ServerTest)
{
  int num_cores = std::thread::hardware_concurrency();

  auto s = std::make_shared<HashServer>("0.0.0.0", 6060, num_cores);
  auto server_thread = std::thread(&start_hash_server, s);

  //Better to not do this, but cutting corners for a simple unit test
  std::this_thread::sleep_for(std::chrono::seconds(1));

  std::vector<std::shared_ptr<std::thread>> connector_threads;

  //Kick off a decent number of connector threads.
  //Let start with 10x the number of hardware cores and make sure they can all connect and read from the server.
  for(int i = 0 ; i < num_cores * 10 ; i++)
  {
    connector_threads.emplace_back(std::make_shared<std::thread>(connect_write_read_test));
  }

  //Wait for them all to finish
  for(auto connector_thread : connector_threads)
  {
    connector_thread->join();
  }

  //Clean up the server
  s->handle_stop();
  server_thread.join();
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#pragma once

#include <string>

/// The common handler for all incoming requests.
class HashRequestHandler
{
public:
  HashRequestHandler() {}
  HashRequestHandler(const HashRequestHandler&) = delete;
  HashRequestHandler& operator=(const HashRequestHandler&) = delete;

  /// Handle a request and produce a reply.
  void handle_request(const std::string& req, std::string& rep);
private:
};

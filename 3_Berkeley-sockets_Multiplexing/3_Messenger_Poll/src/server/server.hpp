#pragma once

#include <inttypes.h>
#include <set>
#include <map>
#include <string>

#include <sys/poll.h>

class MessangerServer final
{
/// fields
private:
  int                         listening_socket_;
  std::set<int>               client_sockets_;
  std::map<int, std::string>  client_to_ip_map_;

  struct pollfd   main_set_[2048];


/// Logic
public:

  void    init(const char *ip, uint16_t port);

  void    run(void);

private:
  void    processClientSockets();

  void    sendToAllClietns(size_t set_size, const std::string& message, int from);

  void    processListeningSocket();  

/// Singleton
public:
  static MessangerServer  instance_;

private:
  MessangerServer() = default;

public:
  ~MessangerServer();

}; //!class MessangerServer final
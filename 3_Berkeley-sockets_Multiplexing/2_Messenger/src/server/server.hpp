#pragma once

#include <inttypes.h>
#include <set>
#include <map>
#include <string>

#include <sys/select.h>

class MessangerServer final
{
/// fields
private:
  int                         listening_socket_;
  std::set<int>               client_sockets_;
  std::map<int, std::string>  client_to_ip_map_;
  
  fd_set  main_read_set_;
  fd_set  main_write_set_;


/// Logic
public:

  void    init(const char *ip, uint16_t port);

  void    run(void);

private:
  void    processClientSockets();

  void    sendToAllClietns(const std::string& message, int from);

  void    processListeningSocket();  

/// Singleton
public:
  static MessangerServer  instance_;

private:
  MessangerServer() = default;

public:
  ~MessangerServer();

}; //!class MessangerServer final
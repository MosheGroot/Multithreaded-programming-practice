#pragma once

#include <inttypes.h>

class EchoServer final
{
/// fields
private:
  int         listening_socket_;


/// Logic
public:

  void    init(const char *ip, uint16_t port);

  void    run(void);


/// Singleton
public:
  static EchoServer  instance_;

private:
  EchoServer() = default;

public:
  ~EchoServer();

}; //!class EchoServer
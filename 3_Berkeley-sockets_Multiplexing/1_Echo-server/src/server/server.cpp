#include "./server.hpp"

#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h> // htos
#include <arpa/inet.h>  // inet_pton
#include <unistd.h>

#include <stdexcept>
#include <iostream>
#include <errno.h>
#include <cstring>

/// Logic

void    EchoServer::init(const char *ip, uint16_t port)
{
  // create socket
  this->listening_socket_ = socket(AF_INET, SOCK_STREAM, 0);

  // enable port reuse
  int optval = 1;
  setsockopt(
    this->listening_socket_, 
    SOL_SOCKET, SO_REUSEPORT,
    &optval, sizeof(optval)
  );


  // create sockaddr_in struct
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = ::htons(port);
  addr.sin_addr.s_addr = ::inet_addr(ip);

  // bind
  if (bind(this->listening_socket_, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    throw std::runtime_error("init exception: can't bind socket");

  // listen
  if (listen(this->listening_socket_, SOMAXCONN) < 0)
    throw std::runtime_error("init exception: can't listen socket");
}


void    EchoServer::run(void)
{
  char                buffer[128] = {0};

  struct sockaddr_in  addr;
  socklen_t           addr_len = sizeof(addr);
  int                 client_fd;

  // accept slien
  while ((client_fd = accept(this->listening_socket_, (struct sockaddr*)&addr, &addr_len)) != -1)
  {
    std::cout << "client " 
      << ::inet_ntoa(addr.sin_addr) << ":" << ::ntohs(addr.sin_port) 
      << " has been connected" << std::endl;
    
    // process messages
    int status;
    while ((status = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
      std::cout << "Recieved: {" << buffer << "}" << std::endl;

      if ((status = send(client_fd, buffer, status + 1, 0)) < 0)
        break;

      ::memset(buffer, '\0', sizeof(buffer));
    }

    // check last status
    if (status == 0)
    { 
      shutdown(client_fd, SHUT_RDWR);
      close(client_fd);
      std::cout << "client disconnected" << std::endl;
    }
    else // status < 0
    {
      close(client_fd);
      std::cerr << "ERROR: " << ::strerror(errno) << std::endl;
    }
  }
}



/// Singleton
EchoServer  EchoServer::instance_;

EchoServer::~EchoServer()
{
  ::close(this->listening_socket_);
}

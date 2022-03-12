#include "./server.hpp"
#include "../utils/network/network.hpp"
#include "../utils/logger/logger.hpp"


#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h> // htos
#include <arpa/inet.h>  // inet_pton
#include <unistd.h>

#include <stdexcept>
#include <iostream>
#include <errno.h>
#include <cstring>
#include <algorithm>
#include <vector>

/// Logic
// General
void    MessangerServer::init(const char *ip, uint16_t port)
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

  // set non-block
  if (Utils::Network::setNonBlocking(this->listening_socket_) < 0)
    throw std::runtime_error("init exception: can't set non-block option");

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

  // add to main set
  main_set_[0].fd = this->listening_socket_;
  main_set_[0].events = POLLIN;
}


void    MessangerServer::run(void)
{
  while (true)
  {
    // Update main set
    size_t index = 1;
    for (auto& s: client_sockets_)
    {
      main_set_[index].fd = s;
      main_set_[index].events = POLLIN | POLLOUT;
      ++index;
    }

    // select
    if (poll(main_set_, 1 + client_sockets_.size(), -1) < 0)
      Utils::Logger::instance_.error(
        ("Poll : " + std::string(::strerror(errno))).c_str()
      );

    // check client sockets
    MessangerServer::processClientSockets();

    // check listenning socket
    MessangerServer::processListeningSocket();
  }
}


void    MessangerServer::processClientSockets()
{
  static char buffer[1024];

  size_t set_size = 1 + client_sockets_.size();
  for (size_t i = 1; i < set_size; ++i)
  {
    if (main_set_[i].revents & POLLIN)
    {
      int socket = main_set_[i].fd;

      // get ip:port of client
      const std::string& from = client_to_ip_map_[socket];

      // recieve
      ::memset(buffer, '\0', sizeof(buffer));
      int recieved = recv(socket, buffer, sizeof(buffer), 0);

      /// Recieved good message
      if (recieved > 1)
      {
        std::string message = std::string(buffer);

        // log
        Utils::Logger::instance_.info(
          ("From " + from + " recieved {" + message + "}").c_str()
        );
        
        // send to all
        MessangerServer::sendToAllClietns(set_size, from + " : " + message, socket);
      }
      else if (recieved == 0 && errno != EAGAIN)
      {
        // disconnect
        if (::shutdown(socket, SHUT_RDWR) < 0)
          Utils::Logger::instance_.error(
            ("shutdown : " + std::string(::strerror(errno))).c_str()
          );

        if (::close(socket) < 0)
          Utils::Logger::instance_.error(
            ("close (disconnect) : " + std::string(::strerror(errno))).c_str()
          );

        client_sockets_.erase(socket);

        // log
        Utils::Logger::instance_.log(
          (from + " has been disconnected").c_str()
        );
      }
      else if (recieved < 0)
      {
        // disconnect
        if (::close(socket) < 0)
          Utils::Logger::instance_.error(
            ("close (error) : " + std::string(::strerror(errno))).c_str()
          );

        // log
        Utils::Logger::instance_.error(::strerror(errno));
      }
      // else EAGAIN or empty message
    }
  }
}

void    MessangerServer::sendToAllClietns(size_t set_size, const std::string& message, int from)
{
  for (size_t i = 1; i < set_size; ++i)
  {
    // is ready for write to and still exists
    if (main_set_[i].revents & POLLOUT &&
      client_sockets_.find(main_set_[i].fd) != client_sockets_.end() &&
      main_set_[i].fd != from) 
    {
      if (send(main_set_[i].fd, message.c_str(), message.length(), 0) < 0)
      {
        Utils::Logger::instance_.error(
          ("Cant send message to " + client_to_ip_map_[main_set_[i].fd] +
          " : " + std::string(::strerror(errno))).c_str()
        );
      }
    }
  }
}


void    MessangerServer::processListeningSocket()
{
  struct sockaddr_in  addr;
  socklen_t           addr_len = sizeof(addr);

  if (main_set_[0].revents & POLLIN)
  {
    // accept and check
    int socket = accept(listening_socket_, (sockaddr*)&addr, &addr_len);
    if (socket < 0)
    {
      Utils::Logger::instance_.error(
        ("Cant accept client : " + std::string(::strerror(errno))).c_str()
      );
      return;
    }

    // make it non-block
    if (Utils::Network::setNonBlocking(socket) < 0)
      throw std::runtime_error("new connection exception: can't set non-block option");
    
    // add to sockets set
    client_sockets_.insert(socket);

    // add ip of client to map
    client_to_ip_map_[socket] = Utils::Network::getReadableIpPort(addr);

    Utils::Logger::instance_.log(
      ("Client " + client_to_ip_map_[socket] + " has been accepted").c_str()
    );
  }
}




/// Singleton
MessangerServer  MessangerServer::instance_;

MessangerServer::~MessangerServer()
{
  for (auto& socket : this->client_sockets_)
  {
    ::shutdown(socket, SHUT_RDWR);
    ::close(socket);
  }

  ::close(this->listening_socket_);
}
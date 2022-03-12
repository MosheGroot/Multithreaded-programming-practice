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
}


void    MessangerServer::run(void)
{
  while (true)
  {
    // Update main sets
    FD_ZERO(&main_read_set_);
    FD_ZERO(&main_write_set_);
    FD_SET(listening_socket_, &main_read_set_);
    for (auto& s: client_sockets_)
    {
      FD_SET(s, &main_read_set_);
      FD_SET(s, &main_write_set_);
    }

    // get max fd
    int max = std::max(listening_socket_, 
      *std::max_element(client_sockets_.begin(), client_sockets_.end())
    );

    // select
    if (select(max + 1, &main_read_set_, &main_write_set_, NULL, NULL) < 0)
      Utils::Logger::instance_.error(
        ("Select : " + std::string(::strerror(errno))).c_str()
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
  bool is_deleted;

  auto it = client_sockets_.begin();
  while (it != client_sockets_.end())
  {
    is_deleted = false;

    if (FD_ISSET(*it, &main_read_set_)) // is ready for read from
    {
      // get ip:port of client
      const std::string& from = client_to_ip_map_[*it];

      // recieve
      ::memset(buffer, '\0', sizeof(buffer));
      int recieved = recv(*it, buffer, sizeof(buffer), 0);

      /// Recieved good message
      if (recieved > 1)
      {
        std::string message = std::string(buffer);

        // log
        Utils::Logger::instance_.info(
          ("From " + from + " recieved {" + message + "}").c_str()
        );
        
        // send to all
        MessangerServer::sendToAllClietns(from + " : " + message, *it);
      }
      else if (recieved == 0 && errno != EAGAIN)
      {
        // disconnect
        if (::shutdown(*it, SHUT_RDWR) < 0)
          Utils::Logger::instance_.error(
            ("shutdown : " + std::string(::strerror(errno))).c_str()
          );

        if (::close(*it) < 0)
          Utils::Logger::instance_.error(
            ("close (disconnect) : " + std::string(::strerror(errno))).c_str()
          );

        is_deleted = true;

        // log
        Utils::Logger::instance_.log(
          (from + " has been disconnected").c_str()
        );
      }
      else if (recieved < 0)
      {
        // disconnect
        if (::close(*it) < 0)
          Utils::Logger::instance_.error(
            ("close (error) : " + std::string(::strerror(errno))).c_str()
          );

        // log
        Utils::Logger::instance_.error(::strerror(errno));
      }
      // else EAGAIN or empty message
    }

    if (is_deleted)
      it = client_sockets_.erase(it);
    else
      ++it;
  }
}

void    MessangerServer::sendToAllClietns(const std::string& message, int from)
{
  for (auto& s : client_sockets_)
  {
    if (FD_ISSET(s, &main_write_set_) && s != from) // is ready for write to
    {     
      if (send(s, message.c_str(), message.length(), 0) < 0)
      {
        Utils::Logger::instance_.error(
          ("Cant send message to " + client_to_ip_map_[s] +
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

  if (FD_ISSET(listening_socket_, &main_read_set_))
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
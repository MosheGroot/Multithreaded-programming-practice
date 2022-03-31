#pragma once

#include <string>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace App::SockUtils
{
  // std::string send & recieve
  std::string   recv_string(int sock);

  void          send_string(int sock, const std::string& str);

  // FD passing utils

  ssize_t   sock_send_fdmsg(int sock, void *buf, ssize_t buflen, int fd);

  ssize_t   sock_recv_fdmsg(int sock, void *buf, ssize_t buflen, int *fd);

} //!namespace App::SockUtils
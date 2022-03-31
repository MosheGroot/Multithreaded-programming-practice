#include "SockUtils.hpp"

#include <stdexcept>
#include <string>
#include <cstring>
#include <errno.h>
#include <string_view>

#include <iostream>
namespace App::SockUtils
{
  /// std::string send & recieve
  
  std::string   recv_string(int sock)
  {
    char    buffer[1024];
    ssize_t recieved = recv(sock, buffer, sizeof(buffer), 0);

    if (recieved < 0)
      throw std::runtime_error("can't recieve data: " + std::string(strerror(errno)));

    return std::string(buffer, recieved);
  }


  void          send_string(int sock, const std::string& str)
  {
    size_t  total_sended = 0;
    size_t  sended;

    while ((sended = send(sock, str.c_str() + total_sended, str.size() - total_sended, 0)) > 0)
    {
      total_sended += sended;
    }

    if (sended < 0)
      throw std::runtime_error("can't send data: " + std::string(strerror(errno)));
  }



  /// FD passing utils

  ssize_t   sock_send_fdmsg(int sock, void *buf, ssize_t buflen, int fd)
  {
    // prepare
    struct iovec    iov;
    struct msghdr   msg;
    struct cmsghdr* cmsg;
    union
    { 
      struct cmsghdr cmsghdr;
		  char control[CMSG_SPACE(sizeof(int))];
	  }	cmsgu;

    iov.iov_base = buf;   iov.iov_len = buflen;
    msg.msg_name = NULL;  msg.msg_namelen = 0;
    msg.msg_iov = &iov;   msg.msg_iovlen = 1;

    msg.msg_control = cmsgu.control;
    msg.msg_controllen = sizeof(cmsgu.control);
    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    *((int *) CMSG_DATA(cmsg)) = fd;

    // send
    ssize_t sended = sendmsg(sock, &msg, 0);
    if (sended < 0)
      throw std::runtime_error("can't send message: " + std::string(strerror(errno)));
    return sended;
  }

  ssize_t   sock_recv_fdmsg(int sock, void *buf, ssize_t buflen, int *fd)
  {
    // prepare
    struct iovec    iov;
		struct msghdr   msg;
		struct cmsghdr* cmsg;
    union
    { 
      struct cmsghdr cmsghdr;
		  char control[CMSG_SPACE(sizeof(int))];
	  }	cmsgu;

		iov.iov_base = buf;   iov.iov_len = buflen;
		msg.msg_name = NULL;  msg.msg_namelen = 0;
		msg.msg_iov = &iov;   msg.msg_iovlen = 1;
    msg.msg_control = cmsgu.control;
    msg.msg_controllen = sizeof(cmsgu.control);

    // recieve
    ssize_t recieved = recvmsg(sock, &msg, 0);
    if (recieved < 0)
      throw std::runtime_error("can't recieve message: " + std::string(strerror(errno)));

    // get fd
    cmsg = CMSG_FIRSTHDR(&msg);
    if (cmsg && cmsg->cmsg_len == CMSG_LEN(sizeof(int)))
		{
			if (cmsg->cmsg_level != SOL_SOCKET)
        throw std::runtime_error("invalid cmsg_level: " + std::string(strerror(errno)));
			if (cmsg->cmsg_type != SCM_RIGHTS)
        throw std::runtime_error("invalid cmsg_type: " + std::string(strerror(errno)));
			*fd = *((int *) CMSG_DATA(cmsg));
		}
		else
    {
			*fd = -1;
    }

    return recieved;
  }

} //!namespace App::SockUtils
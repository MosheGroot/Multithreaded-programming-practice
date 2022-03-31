#include "EchoWorker.hpp"

#include "../SockUtils/SockUtils.hpp"
#include <iostream>
namespace App
{

  std::string   EchoWorker::call(int fd)
  {
    char buf[1] = "";

    // forward the inpurt descriptor
    SockUtils::sock_send_fdmsg(this->socket_, buf, sizeof(buf), fd);

    // get anwser
    std::string out = SockUtils::recv_string(this->socket_);
    return out;
  }


  void          EchoWorker::workerFunction()
  {
    char  buffer[1024];
    int   fd = 0;

    while (true)
    {
      SockUtils::sock_recv_fdmsg(this->socket_, buffer, sizeof(buffer), &fd);
      if (fd < 0)
        continue;

      std::string echo_string;
      int         readed;
      while ((readed = read(fd, buffer, sizeof(buffer))) > 0)
      {
        echo_string += std::string_view(buffer, readed);
      }
      close(fd);

      if (readed < 0)
        SockUtils::send_string(this->socket_, "read error");
      else
        SockUtils::send_string(this->socket_, echo_string);
    }
  }

} //!namespace App
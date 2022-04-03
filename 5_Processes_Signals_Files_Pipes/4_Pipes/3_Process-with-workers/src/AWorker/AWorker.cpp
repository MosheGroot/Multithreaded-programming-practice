#include "AWorker.hpp"

#include <unistd.h>
#include <cstdlib>

#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <iostream>

#include <stdexcept>
#include <string>
#include <cstring>
#include <errno.h>

namespace App
{
  // Static fields
  std::vector<int>  AWorker::opened_sockets_;

  // Constructor
  AWorker::AWorker()
    : pid_(-1), socket_(-1)
  {}

  AWorker::~AWorker()
  {
    if (this->socket_ == -1)
      return;
    close(this->socket_);

    if (this->pid_ == -1)
      return;

    kill(this->pid_, SIGTERM);

    int status;
    waitpid(this->pid_, &status, 0);
    std::cout << "[Worker exit with status " << WEXITSTATUS(status) << "]" << std::endl;
  }


  // Logic
  void    AWorker::exec()
  {
    // create socker pair
    int socket_pair[2];
    int status = socketpair(
      AF_UNIX,
      SOCK_STREAM,
      0,
      socket_pair
    );

    if (status != 0)
      throw std::runtime_error("can't create socker pair: " + std::string(strerror(errno)));


    // fork
    this->pid_ = fork();
    if (this->pid_ == 0)
    {
      this->socket_ = socket_pair[1];
      close(socket_pair[0]);
      for (auto i : this->opened_sockets_) // delete sockets between workers
        close(i);

      this->workerFunction();
      exit(0);
    }
    else if (this->pid_ < 0)
      throw std::runtime_error("can't fork the process: " + std::string(strerror(errno)));

    // success
    this->socket_ = socket_pair[0];
    close(socket_pair[1]);
    this->opened_sockets_.push_back(this->socket_);
  }

} //!namespace App
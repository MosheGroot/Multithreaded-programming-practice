#pragma once

#include <string>

#include <netinet/in.h> 

namespace Utils::Network
{
  
  int         setNonBlocking(int fd);

  std::string getReadableIpPort(struct sockaddr_in& addr);

} //!namespace Utils::Network
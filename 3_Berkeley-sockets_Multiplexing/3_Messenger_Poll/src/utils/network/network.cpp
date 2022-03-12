#include "./network.hpp"

#include <sstream>
#include <fcntl.h>
#include <arpa/inet.h>


namespace Utils::Network
{
  // Utils::Network
  int          setNonBlocking(int fd)
  {
    int flags;
  #if defined(O_NONBLOCK)
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
      flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  #else
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
  #endif
  } 


  std::string   getReadableIpPort(struct sockaddr_in& addr)
  {
    std::stringstream ss;
    
    ss << ::inet_ntoa(addr.sin_addr) << ":" << ::ntohs(addr.sin_port);

    return ss.str();
  }

} //!namespace Utils::Network
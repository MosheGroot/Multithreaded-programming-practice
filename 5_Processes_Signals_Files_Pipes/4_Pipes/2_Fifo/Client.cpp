#include <iostream>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Settings.hpp"

void  fifoEcho(int fd_in)
{
  static char buffer[8];

  ssize_t readed;
  while ((readed = read(fd_in, buffer, sizeof(buffer))) > 0)
  {
    if (write(STDOUT_FILENO, buffer, readed) < 0)
      std::cerr << "Write error" << std::endl;
  }

  if (readed < 0)
    std::cerr << "Read error" << std::endl;
}

int   main()
{
  int fd = open(FIFO_FILENAME, O_RDONLY);
  if (fd < 0)
  {
    std::cerr << "Open error" << std::endl;
    return 1;
  }

  fifoEcho(fd);
  close(fd);

  return 0;
}
#include <iostream>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Settings.hpp"

void  stdinEcho(int fd_out)
{
  static char buffer[8];

  ssize_t readed;
  while ((readed = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
  {
    if (write(fd_out, buffer, readed) < 0)
      std::cerr << "Write error" << std::endl;
  }

  if (readed < 0)
    std::cerr << "Read error" << std::endl;
}

int   main()
{
  if (mkfifo(FIFO_FILENAME, 0666))
    std::cerr << "mkfifo error" << std::endl;

  int fd = open(FIFO_FILENAME, O_WRONLY);
  stdinEcho(fd);
  close(fd);

  return 0;
}
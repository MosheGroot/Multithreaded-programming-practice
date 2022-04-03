#include <iostream>
#include <array>
#include <unistd.h>
#include <fcntl.h>

#include "EchoWorker/EchoWorker.hpp"


int   main()
{
  // create workers
  std::array<App::EchoWorker, 4>  workers;
  for (auto& worker: workers)
    worker.exec();

  // make requests to workers
  while (true)
  {
    for (size_t i = 0; i < workers.size(); ++i)
    {
      int fd = open("testfile.txt", O_RDONLY);
      if (fd < 0)
        throw std::runtime_error("open error");

      std::cout << "Worker #" << i << ":\n{"
                << workers[i].call(fd) << "}\n" << std::endl;

      close(fd);
    }

    std::cin.get();
  }
}
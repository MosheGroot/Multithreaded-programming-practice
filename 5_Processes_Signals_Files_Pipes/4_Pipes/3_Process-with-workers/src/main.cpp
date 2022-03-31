#include <iostream>
#include <array>
#include <unistd.h>
#include <fcntl.h>

#include "EchoWorker/EchoWorker.hpp"

template <size_t S>
void   initEchoWorkers(std::array<App::EchoWorker *, S>& workers)
{
  for (auto& worker: workers)
  {
    worker = new App::EchoWorker();
    worker->exec();
  }
}

int   main()
{
  // create workers
  std::array<App::EchoWorker *, 4>  workers;

  initEchoWorkers(workers);


  // make requests to workers
  while (true)
  {
    int fd = open("testfile.txt", O_RDONLY);
    if (fd < 0)
      throw std::runtime_error("open error");

    for (auto& worker: workers)
      std::cout << worker->call(fd) << std::endl;
    close(fd);

    std::cin.get();
  }
}
#include <iostream>
#include <cstdlib>

#include "server/server.hpp"

int   main(int argc, const char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Invalid number of arguments!\n"
      "Please, provide ip and port to listen." << std::endl;
    return 1;
  }


  try
  {
    MessangerServer::instance_.init(argv[1], ::atoi(argv[2]));
    MessangerServer::instance_.run();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  

  return 0;
}
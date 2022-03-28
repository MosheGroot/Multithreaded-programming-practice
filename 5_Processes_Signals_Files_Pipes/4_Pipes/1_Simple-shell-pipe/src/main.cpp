#include <iostream>

#include "Command/Command.hpp"
#include "Executor/Executor.hpp"

int   main()
{
  // get line
  std::string commandline;
  std::getline(std::cin, commandline);

  // parse
  PipeApp::CommandList  parsed_list(commandline);

  // exec
  PipeApp::Executor::exec(parsed_list);

  return 0;
}
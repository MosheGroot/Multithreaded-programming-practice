#pragma once

#include <iostream>
#include <unistd.h>
#include "../Command/Command.hpp"

namespace PipeApp::Executor
{

  void    exec(CommandList& command, int fd_in=STDIN_FILENO, int fd_out=STDOUT_FILENO);

} //!namespace PipeApp::Executor
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <cstring>

#define EXEC_COMMAND "cat"

int   exec_echo(char *argv[])
{
  pid_t pid = fork();

  if (!pid) 
  {
    /// Child proccess
    sleep(1);
    execvp(EXEC_COMMAND, argv);

    std::cout << "Can't execute echo: " << ::strerror(errno) << std::endl;
    exit(2);
  }
  else if (pid > 0) 
  {
    /// Parent process: success
    std::cout << "fork was successfull!" << std::endl;

    // wait
    int status;
    waitpid(pid, &status, 0);

    // check status
    if (status == 0)
    {
      std::cout << "Echo execution done successfully!" << std::endl;
      return 0;
    }
    else
    {
      std::cout << "Echo execution error..." << std::endl;
      return 2;
    }
  }
  else
  {
    /// Parent process: exec failed
    std::cout << "Can't execute echo: " << ::strerror(errno) << std::endl;
    return 1;
  }
}


int   main(int argc, char* argv[])
{
  argv[0] = (char *)EXEC_COMMAND;
  return exec_echo(argv);
}



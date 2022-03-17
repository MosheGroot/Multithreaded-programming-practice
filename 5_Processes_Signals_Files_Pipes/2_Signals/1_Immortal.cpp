#include <iostream>
#include <fstream>
#include <unistd.h>
#include <signal.h>

#define PID_STOREPATH "./pid"


int   create_immortal_process(void)
{
  pid_t pid = fork();
  if (pid == 0)
  {
    // create immortal proccess
    sigignore(SIGTERM);
    sigignore(SIGINT);

    // infinite loop
    while (true)
      pause(); 
  }
  else if (pid < 0)
  {
    // Errors
    std::cerr << "Fork failed" << std::endl;
    return 1;
  }

  // save child PID to file (required by task)
  std::ofstream fout(PID_STOREPATH);
  fout << pid;
  fout.close();
  std::cout << "Child's PID has been written to file "
    << PID_STOREPATH << std::endl;

  // wait
  int status;
  wait(&status);
  std::cout << "Child process closed" << std::endl;

  return 0;
}


int   main(void)
{
  return create_immortal_process();
}
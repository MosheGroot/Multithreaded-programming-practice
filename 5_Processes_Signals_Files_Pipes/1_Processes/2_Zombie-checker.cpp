#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <random>

#define CHILDS_NUMBER 10
#define WORK_DELAY 2

///////////////// CHILD

int   childs_code(int number)
{
  /// set deed
  std::srand(std::time(NULL));   // set personal seed
  sleep(CHILDS_NUMBER - number); // wait for other processes

  /// rand
  int random_duration = 5 + std::rand() % 20;
  std::cout << "\t[" << getpid() << "] is waiting for " 
            << random_duration << " seconds" << std::endl;
  
  /// sleep
  sleep(random_duration);

  /// exit
  exit(0);
}



///////////////// FORK

int   create_childs()
{
  pid_t pid;

  std::cout << "Creating child processes..." << std::endl;
  for (int i = 0; i < CHILDS_NUMBER; ++i)
  {
    pid = fork();
    if (!pid) 
    {
      childs_code(i);
    }
    else if (pid < 0)
    {
      std::cerr << "Fork failed!" << std::endl;
      return 1;
    }

    sleep(1);
  }
  std::cout << "Creating done...\n" << std::endl;

  return 0;
}




///////////////// MAIN PROCESS
//////////// CHECK

bool  check_zombies()
{
  /// begin
  std::cout << "Start zombie checking..." << std::endl;
  
  // wait for processes
  int pid, status;
  while((pid = waitpid(-1, &status, WNOHANG)) > 0)
  {
    /* exited process */
    std::cout << "\tProcess with PID[" << pid 
              << "] exited with status [" << WEXITSTATUS(status) 
              << "]" << std::endl;
  }
  
  // check last PID
  if (pid < 0)              /* error or no childs */
  {
    if (errno != ECHILD)
      std::cerr << "Waitpid error : " << ::strerror(errno) << std::endl;
    
    std::cout << "Zombie check has been done. "
                 "There is no child processes left" << std::endl;
    return false;
  }
  else                      /* no exited processes */
  {
    std::cout << "Zombie check has been done. "
                 "There is still some running child processes." << std::endl;
    return true;
  }
}


//////////// WORK

void  some_work()
{
  int counter = 0;
  while (counter < CHILDS_NUMBER)
  {
    std::cout << "\n*Doing some hard work*" << std::endl;
    sleep(WORK_DELAY);

    if (!check_zombies())
    {
      std::cout << "\n*Hard work has been done*" << std::endl;
      return;
    }
  }
}


///////////////// MAIN

int   main()
{
  create_childs();
  some_work();
}
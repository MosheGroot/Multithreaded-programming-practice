#include <iostream>
#include <signal.h>
#include <unistd.h>

int*  global_to_free;


void  cleanup_handler(int status)
{
  std::cout << "\n{HANDLER WAS CALLED}\n" << std::endl;
  delete[] global_to_free;
}


void  configure_signal()
{
  // set up sigset (block all)
  sigset_t  sigset;
  sigemptyset(&sigset);
  
  // set
  struct sigaction act;
  act.sa_mask = sigset;
  act.sa_flags = SA_RESETHAND | SA_RESTART; // avoid double free + continue pause
  act.__sigaction_u.__sa_handler = cleanup_handler;

  sigaction(SIGINT, &act, NULL);
}


int   main()
{
  global_to_free = new int[1024];

  configure_signal();

  std::cout << "Waiting for ^C..." << std::endl;
  pause();
  
  std::cout << "Exiting programm..." << std::endl;
  return 0;
}
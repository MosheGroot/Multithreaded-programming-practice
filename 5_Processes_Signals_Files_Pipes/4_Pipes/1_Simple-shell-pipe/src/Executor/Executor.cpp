#include "Executor.hpp"

#include "../Env/Env.hpp"

#include <iostream>

#include <sys/wait.h>
#include <stdexcept>
#include <errno.h>
#include <cstring>
#include <vector>


namespace PipeApp::Executor
{
  /******************/
  /*     Utils      */
  /******************/

  static void  readFromFd(int fd_in, int fd_out)
  {
    static char buffer[1024];
    ssize_t     readed;

    while ((readed = read(fd_in, buffer, sizeof(buffer))) > 0)
    {
      write(fd_out, buffer, readed);
    }

    if (readed < 0)
    {
      std::cerr << "[WARNING]: cant read from pipe" << std::endl;
    }
  }


  static void   waitProcess(int pid)
  {
    int status;
    if (waitpid(pid, &status, 0) < 0)
    {
      throw std::runtime_error(::strerror(errno));
    }

    if (WEXITSTATUS(status) != EXIT_SUCCESS)
    {
      std::cerr << "[WARNING]: process exited with status code {" 
                << WEXITSTATUS(status) << "}" << std::endl;
    }
  }


  /*****************************************************************/
  /*     Exec with input from `fd_in` and output to `fd_out`      */
  /*****************************************************************/

  static int   execCommand(Command& command, int fd_in, int pipe_fds[2])
  {
    // fork
    int pid = fork();
    if (pid == 0)
    {
      if (fd_in != STDIN_FILENO)
      {
        close(STDIN_FILENO);
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
      }
      close(STDOUT_FILENO);
      dup2(pipe_fds[1], STDOUT_FILENO);
      close(pipe_fds[1]);
      close(pipe_fds[0]);

      execve(command.getCommand(), command.getArgv(), environ);
      exit(1); // exec error
    }
    else if (pid < 0)
    {
      throw std::runtime_error(::strerror(errno));
    }

    return pid;
  }

  void  exec(CommandList& commandlist, int fd_in, int fd_out)
  {
    std::vector<int>  pids;
    int               pipe_fds[2];
    int               prev_fd = fd_in;

    for (auto& command: commandlist)
    {
      // pipe
      if (pipe(pipe_fds) != 0)
      {
        if (prev_fd != STDIN_FILENO)
          close(prev_fd);
        for (auto& pid : pids)
          waitProcess(pid);
        throw std::runtime_error(::strerror(errno));
      }

      // exec single command
      try
      {
        pids.push_back(
          execCommand(command, prev_fd, pipe_fds)
        );
      }
      catch(const std::exception& e)
      {
        if (prev_fd != STDIN_FILENO)
          close(prev_fd);
        close(pipe_fds[1]);
        close(pipe_fds[0]);
        for (auto& pid : pids)
          waitProcess(pid);
        throw e;
      }

      // save pipe's read fd
      if (prev_fd != STDIN_FILENO)
        close(prev_fd);
      close(pipe_fds[1]);
      prev_fd = pipe_fds[0];
    }

    // get output
    readFromFd(prev_fd, fd_out);
    close(prev_fd);

    // wait
    for (auto& pid : pids)
      waitProcess(pid);
  }

} //!namespace PipeApp::Executor
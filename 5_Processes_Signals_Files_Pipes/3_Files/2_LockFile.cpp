#include <iostream>
#include <cstdlib>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define LOCK_FILE   ".lock"

bool    is_dir(const char *path)
{
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISDIR(path_stat.st_mode);
}

bool    lock_dir(const char *dirpath)
{
  // Is a directory
  if (!is_dir(dirpath))
  {
    std::cerr << "Provided path is not a directory!" << std::endl;
    return false;
  }

  // create lock file full path
  std::string lockfile_fullpath(dirpath);
  lockfile_fullpath.push_back('/');
  lockfile_fullpath += LOCK_FILE;

  // creating file
  int fd;
  if ((fd = open(lockfile_fullpath.c_str(), O_WRONLY | O_CREAT | O_EXCL, 000)) == -1)
  {
    perror("lock file open error");
    return false; // failed
  }
  else
  {
    close (fd);   // success
    return true;
  }
}

bool    unlock_dir(const char *dirpath)
{
  // create lock file full path
  std::string lockfile_fullpath(dirpath);
  lockfile_fullpath.push_back('/');
  lockfile_fullpath += LOCK_FILE;
  
  if (unlink(lockfile_fullpath.c_str()) < 0)
  {
    perror("lock file unlink error");
  }

  return true; // success always
}

void    print_help(void)
{
  std::cout << "Please run the program as follows:\n"
               "\t./a.out workdir_path" << std::endl;
}

const char*  parse_agrv(int argc, char **argv)
{
  // help
  if (argc == 1)
  {
    print_help();
    exit(0);
  }

  // invalid arguments
  if (argc > 2)
  {
    std::cerr << "Wrong nubmer of arguments!" << std::endl;
    print_help();
    exit(1);
  }

  // normal arguments
  return argv[1];
}

int   main(int argc, char **argv)
{
  const char *workdir = parse_agrv(argc, argv);

  if (lock_dir(workdir))
    std::cout << "Lock was successfull!" << std::endl;
  else
    return 1;

  std::cout << "\n\tDoing some work with {" << workdir << "} directory\n\n"
               "\tPress enter to end this work...\n" << std::endl;
  std::cin.get();

  if (unlock_dir(workdir))
    std::cout << "Unlock was successfull!" << std::endl;
  else
    return 1;

  return 0;
}
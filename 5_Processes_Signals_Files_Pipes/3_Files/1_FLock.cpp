#include <iostream>
#include <cstdlib>
#include <cstring>

#include <unistd.h>
#include <fcntl.h>

#define OP_READ  1
#define OP_WRITE 2

#define OP_READ_STOP    0
#define OP_READ_READ    1
#define OP_READ_UNLOCK  2
#define OP_READ_LOCK    3

#define OP_WRITE_STOP   0
#define OP_WRITE_WRITE  1

/***********************
*      READ SHARED     *
***********************/
static void    read_and_print(int fd)
{
  static char  buffer[1024];

  // seek to the beginning
  if (lseek(fd, 0, SEEK_SET) < 0)
  {
    perror("lseek error");
    return;
  }
  
  // read
  std::cout << '{';
  int readed;
  while ((readed = read(fd, buffer, sizeof(buffer))) > 0)
  {
    std::cout.write(buffer, readed);
  }
  if (readed < 0)
  {
    perror("read error");
  }
  std::cout << "}\n" << std::endl;
}

static void   print_read_commands(void)
{
  std::cout << "Please enter one of the next options:\n"
               "\t" << OP_READ_READ << ". Read file and print\n"
               "\t" << OP_READ_UNLOCK << "2. Unlock the file\n"
               "\t" << OP_READ_LOCK << "3. Shared lock the file\n"
               "\t" << OP_READ_STOP << "4. End the program\n"
            << std::endl;
}

void    readfile_shared(const char *filename)
{
  // shared lock
  int   fd = open(filename, O_RDONLY | O_SHLOCK); 
  if (fd < 0)
  {
    perror("open error");
    return;
  }

  int command = OP_READ_READ;
  while (command != OP_READ_STOP)
  {
    print_read_commands();
    std::cin >> command;

    if (command == OP_READ_READ)
    {
      read_and_print(fd);
    }
    else if (command == OP_READ_UNLOCK)
    {
      if (flock(fd, LOCK_UN) < 0)
        perror("flock UN error");
    }
    else if (command == OP_READ_LOCK)
    {
      if (flock(fd, LOCK_SH) < 0)
        perror("flock SH error");
    }
    else if (command != OP_READ_STOP)
    {
      std::cout << "Wrong command! Try again..." << std::endl;
    }
  }
}

/***********************
*    WRITE EXCLUSIVE   *
***********************/
void    write_to_file(int fd, const char *data)
{
  // lock exclusive
  std::cout << "Waiting to lock the file..." << std::endl;
  if (flock(fd, LOCK_EX) < 0)
  {
    perror("flock EX error");
    return;
  }
  std::cout << "Lock was successfull\n" << std::endl;

  if (data)
  {
    int data_len = strlen(data);
    int writed = 0;
    while (writed < data_len)
    {
      int status = write(fd, data + writed, data_len - writed);
      if (status < 0)
      {
        perror("write error");
        return;
      }
      writed += status;
    }
  }

  if (flock(fd, LOCK_UN) < 0)
  {
    perror("flock UN error");
  }
}

static void   print_write_commands(void)
{
  std::cout << "Please enter one of the next options:\n"
               "\t" << OP_WRITE_WRITE <<"1. Write data (from arguments)\n"
               "\t" << OP_WRITE_STOP <<"2. End the program\n"
            << std::endl;
}

void    writefile_exclusive(const char *filename, const char *data)
{
  // shared lock
  int   fd = open(filename, O_WRONLY); 
  if (fd < 0)
  {
    perror("open error");
    return;
  }

  int command = OP_WRITE_WRITE;
  while (command != OP_WRITE_STOP)
  {
    print_write_commands();
    std::cin >> command;

    if (command == OP_WRITE_WRITE)
    {
      write_to_file(fd, data);
    }
    else if (command != OP_WRITE_STOP)
    {
      std::cout << "Wrong command! Try again..." << std::endl;
    }
  }
}

/***********************
*   ARGUMENTS PARSING  *
***********************/
void  print_help(void)
{
  std::cout << "Please run the programm as follow:\n"
               "\t./a.out path_to_file [read]\n"
               "\tor\n"
               "\t./a.out path_to_file write [data_to_write]"
            << std::endl;
}

int  parse_argv(int argc, char **argv)
{
  if (argc == 1 || (argc == 2 && strcmp(argv[1], "--help")))
  {
    print_help();
    exit(0);
  }

  if (argc == 3 && !strcmp(argv[2], "read"))
  {
    return OP_READ;
  }
  else if (3 <= argc && argc <= 4 && !strcmp(argv[2], "write"))
  {
    return OP_WRITE;
  }
  else
  {
    std::cerr << "Wrong arguments!\n" << std::endl;
    print_help();
    exit(1);
  }
}

int   main(int argc, char **argv)
{
  int   operation = parse_argv(argc, argv);
  if (operation == OP_READ)
  {
    readfile_shared(argv[1]);
  }
  else if (operation == OP_WRITE)
  {
    writefile_exclusive(argv[1], argv[3]);
  }

  return 0;
}
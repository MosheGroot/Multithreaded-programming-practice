#include "Env.hpp"

#include <cstring>
#include <string>
#include <algorithm>

#include <sys/types.h>
#include <sys/stat.h>


namespace PipeApp::Env
{
  
  static bool   isExecutable(const char *path)
  {
    struct stat sb;
    if (stat(path, &sb) != 0)
      return false;
    return sb.st_mode & S_IXUSR;
  }


  std::string   getFromPath(const std::string& command)
  {
    size_t i = 0;
    for (; environ[i]; ++i)
      if (::strncmp(environ[i], "PATH=", 5) == 0)
        break;

    // original
    if (!environ[i])
      return command;

    // with path
    std::string   path_value(environ[i] + 5);
    auto          begin_it = path_value.begin();
    auto          end_it = std::find(begin_it, path_value.end(), ':');

    while (begin_it != path_value.end())
    {
      
      std::string temp_path(begin_it, end_it);
      if (temp_path.back() != '/')
        temp_path.push_back('/');
      temp_path += command;

      if (isExecutable(temp_path.c_str()))
        return temp_path;

      begin_it = (end_it != path_value.end()) ? std::next(end_it) : end_it;
      end_it = std::find(begin_it, path_value.end(), ':');
    }
    return command;
  }

} //!namespace PipeApp::Env
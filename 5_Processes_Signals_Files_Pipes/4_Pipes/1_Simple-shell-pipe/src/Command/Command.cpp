#include "Command.hpp"

#include "../Env/Env.hpp"

#include <algorithm>

namespace PipeApp
{

  ////////////////////
  //    Command     //
  ////////////////////

  Command::Command(const std::string::const_iterator& begin, 
            const std::string::const_iterator& end)
  {
    // Get command
    auto begin_it = std::find_if(begin, end,
      [] (char c) { return !::isspace(c); });
    auto end_it = std::find_if(begin_it, end,
      [] (char c) { return ::isspace(c); });

    {
      std::string temp_command = Env::getFromPath(std::string(begin_it, end_it));
      this->command_ = new char[temp_command.size() + 1];
      ::memcpy(this->command_, temp_command.c_str(), temp_command.size() + 1);
    }


    // Get argv
    std::vector<std::string> argv;
    argv.push_back(std::string(begin_it, end_it));
    while (end_it != end)
    {
      begin_it = std::find_if(end_it, end,
        [] (char c) { return !::isspace(c); });
      end_it = std::find_if(begin_it, end,
        [] (char c) { return ::isspace(c); });

      if (begin_it != end_it)
        argv.push_back(std::string(begin_it, end_it));
    }

    this->argv_ = new char*[argv.size() + 1];
    for (size_t i = 0; i < argv.size(); ++i)
    {
      this->argv_[i] = new char[argv[i].size() + 1];
      ::memcpy(this->argv_[i], argv[i].c_str(), argv[i].size() + 1);
    }
    this->argv_[argv.size()] = nullptr;
  }

  Command::Command(const std::string& command)
    : Command(command.cbegin(), command.cend())
  {}

  Command::Command(Command&& old)
    : command_(old.command_), argv_(old.argv_)
  {
    old.command_ = nullptr;
    old.argv_ = nullptr;
  }

  Command& Command::operator=(Command&& old)
  {
    if (this == &old)
      return *this;

    this->command_ = old.command_; old.command_ = nullptr;
    this->argv_ = old.argv_; old.argv_ = nullptr;
    return *this;
  }

  Command::~Command()
  {
    delete[] this->command_;

    if (this->argv_)
    {
      size_t i = 0;
      while (this->argv_[i])
        delete[] this->argv_[i++];
      delete[] this->argv_;
    }
  }


  const char*   Command::getCommand()  const
  {
    return this->command_;
  }


  char* const*  Command::getArgv()
  {
    return this->argv_;
  }



  ////////////////////
  //  Command List  //
  ////////////////////

  CommandList::CommandList(const std::string& commandline)
  {
    // init iterators
    auto begin_it = commandline.cbegin();
    auto end_it = std::find(commandline.begin(), commandline.end(), '|');
    
    // parse commands
    while (begin_it != commandline.cend())
    {
      this->container_.emplace_back(begin_it, end_it);
      begin_it = (end_it != commandline.end()) ? std::next(end_it) : end_it;
      end_it = std::find(begin_it, commandline.end(), '|');
    }
  }

  CommandList::CommandList(CommandList&& old)
    : container_(std::move(old.container_))
  {}

  CommandList& CommandList::operator=(CommandList&& old)
  {
    if (this == &old)
      return *this;

    this->container_ = std::move(old.container_);
    return *this;
  }

  CommandList::iterator  CommandList::begin()
  {
    return this->container_.begin();    
  }
    
  CommandList::iterator  CommandList::end()
  {
    return this->container_.end();
  }

} //!namespace PipeApp
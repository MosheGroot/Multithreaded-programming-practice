#pragma once

#include <vector>
#include <string>

namespace PipeApp
{
  
  class Command final
  {
  // Fields
  private:
    char*   command_;
    char**  argv_;

  // Constructor and destructor
  public:
    Command(const std::string::const_iterator& begin, 
            const std::string::const_iterator& end);
    Command(const std::string& command);

    Command(Command&& old);
    Command& operator=(Command&& old);

    Command(const Command& other) = delete;
    Command& operator=(const Command& other) = delete;

    ~Command();

  // Logic
    const char*   getCommand()  const;
    char* const*  getArgv();

  }; //!class Command final



  class CommandList final
  {
  // Fields
  public:
    using container = std::vector<Command>;
    using iterator  = typename container::iterator;

  private:
    std::vector<Command>  container_;

  // Constructor and destructor
  public:
    CommandList(const std::string& commandline);
    
    CommandList(CommandList&& old);
    CommandList& operator=(CommandList&& old);

    CommandList(const CommandList& other) = delete;
    CommandList& operator=(const CommandList& other)  = delete;

    ~CommandList() = default;

  // Logic
    iterator  begin();
    iterator  end();

  }; //!class CommandList final

} //!namespace PipeApp
#pragma once

#include <mutex>
#include <memory>

namespace Utils
{
  class Logger final
  {
  // Singleton 
  public:
    static Logger       instance_;

    ~Logger() = default;

  private:
    Logger() = default;

  // Logic
  public:
    void  error(const char* ptr);

    void  info(const char* msg);

    void  debug(const char* msg);

    void  log(const char* msg);

  private:
    void  printMsg(const char* ptr, const char* prompt, const char* colors, std::ostream& os);

  }; //!class Logger final
} //!namespace Utils
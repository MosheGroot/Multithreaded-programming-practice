#include "./logger.hpp"

#include <iostream>
#include <iomanip>

#include "../time/time.hpp"

namespace Utils
{
  // Signleton
  Logger  Logger::instance_;


  // Logic
  void  Logger::error(const char* msg)
  {
    printMsg(msg, "ERROR", "\x1B[1;31m", std::cerr);
  }
  
  void  Logger::info(const char* msg)
  {
    printMsg(msg, "INFO", "\x1B[32m", std::cerr);
  }

  void  Logger::debug(const char* msg)
  {
    printMsg(msg, "DEBUG", "\x1B[33m", std::cerr);
  }

  void  Logger::log(const char* msg)
  {
    printMsg(msg, "LOG", "\x1B[249m", std::cerr);
  }


  void  Logger::printMsg(const char* msg, const char* prompt, const char* colors, std::ostream& os)
  {
    // timestamp
    os << "[" << Utils::Time::getCurrentTimestamp() << "] ";

    // prompt
    if (colors)
      os << colors;

    if (prompt)
      os << '[' << std::setiosflags(std::ios_base::left)
        << std::setw(5) << prompt << "] ";

    if (colors)
      os << "\x1B[m";

    // message
    os << msg << std::endl;
  }


} //!namespace Utils
#pragma once

#include "../AWorker/AWorker.hpp"

#include <string>

namespace App
{

  class EchoWorker : public AWorker
  {
  public:
    std::string     call(int fd);

  protected:
    virtual void    workerFunction() override;

  }; //!class EchoWorker : public AWorker

} //!namespace App
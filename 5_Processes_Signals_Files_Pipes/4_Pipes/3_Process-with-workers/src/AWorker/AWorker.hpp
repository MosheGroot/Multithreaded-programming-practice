#pragma once

#include <vector>

namespace App
{
 
  class AWorker
  {
  // Fields
  private:
    static std::vector<int> opened_sockets_;

  protected:
    int   pid_;
    int   socket_;

  // Constructors and destructor
  public:
    AWorker();

    AWorker(const AWorker& other) = delete;
    AWorker(AWorker&& old) = delete;

    virtual ~AWorker();

  // Logic
  public:
    void    exec();

  protected:
    virtual void    workerFunction() = 0;

  }; //! AWorker

} //!namespace App
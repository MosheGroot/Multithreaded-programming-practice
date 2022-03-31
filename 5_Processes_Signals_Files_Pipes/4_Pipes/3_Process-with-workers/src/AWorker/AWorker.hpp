#pragma once

namespace App
{
 
  class AWorker
  {
  // Fields
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
#include <iostream>
#include <string>

class StringPointer {
private:
  std::string *ptr_;

public:
  std::string *operator->()
  {
    if (!this->ptr_)
      this->ptr_ = new std::string();
    return this->ptr_;
  }

  operator std::string*()
  {
    if (!this->ptr_)
      this->ptr_ = new std::string();
    return this->ptr_;
  }

  StringPointer(std::string *Pointer)
    : ptr_(Pointer)
  {}

  ~StringPointer()
  {
    if (this->ptr_)
      delete this->ptr_;
  }
}; //!class StringPointer


// main test
int main()
{
  StringPointer sp1(new std::string("Hello, world!"));
  StringPointer sp2(NULL);

  std::cout << sp1->length() << std::endl;
  std::cout << *sp1 << std::endl;
  std::cout << sp2->length() << std::endl;
  std::cout << *sp2 << std::endl;

  return 0;
}
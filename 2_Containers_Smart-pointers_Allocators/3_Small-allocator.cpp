#include <iostream>
#include <vector>
#include <algorithm>
#include <memory.h>


struct MemoryBlock
{
  uint64_t  address;
  uint64_t  size;
};


class SmallAllocator
{
private:
// fields
  char                      memory_[1048576];
  std::vector<MemoryBlock>  blocks_;



public:
// alocator and destructor
  SmallAllocator() = default;

  ~SmallAllocator() = default;



// logic
  void *Alloc(unsigned int size)
  {
    uint64_t address = 0;

    // get adress
    for (size_t i = 0; i < blocks_.size(); ++i)
    {
      if (i > 0)    // memory between two allocated blocks
      {
        address = blocks_[i - 1].address + blocks_[i - 1].size;
        
        if (size <= blocks_[i].address - address)
          break;
      }
      else if (blocks_.size() > 1)  // first, but not alone
        continue;

      if (i == blocks_.size() - 1) // (first with blocks amount = 1) or (last with blocks amount > 1)
      {
        address = blocks_[i].address + blocks_[i].size;

        if (size > sizeof(memory_) - address)
          throw std::bad_alloc();
      }
    }

    // push block and return
    blocks_.push_back( {address, size} );
    return static_cast<void *>(memory_ + address);
  };


  void *ReAlloc(void* const ptr, unsigned int size)
  {
    /// get relative adress in memory
    uint64_t to_find_address = static_cast<uint64_t>(
      static_cast<char *>(ptr) - memory_
    );

    /// find block
    auto it = std::find_if(blocks_.begin(), blocks_.end(), 
      [&] (const MemoryBlock& block) { return block.address == to_find_address; });

    if (it == blocks_.end())
      throw std::invalid_argument("such block was not found");
    
    /// try to resize
    auto it_next = std::next(it);
    if (it_next != blocks_.end())
    {
      if (size <= it_next->address - it->address)
      {
        it->size = size;
        return ptr;
      }
    }
    else // `it` is last block
    {
      if (size <= sizeof(memory_) - it->address)
      {
        it->size = size;
        return ptr;
      }
    }

    /// else reallocate
    uint64_t old_size = it->size;
    void *new_ptr = Alloc(size);
    ::memcpy(new_ptr, ptr, old_size);
    Free(ptr);

    return new_ptr;
  };


  void Free(void* const ptr)
  {
    /// get relative adress in memory
    uint64_t to_find_address = static_cast<uint64_t>(
      static_cast<char *>(ptr) - memory_
    );

    /// find block
    auto it = std::find_if(blocks_.begin(), blocks_.end(), 
      [&] (const MemoryBlock& block) { return block.address == to_find_address; });

    if (it == blocks_.end())
      throw std::invalid_argument("such block was not found");
 
    /// delete
    blocks_.erase(it);
  };

}; //!class SmallAllocator



int   main(void)
{
  SmallAllocator A1;
  int * A1_P1 = (int *) A1.Alloc(sizeof(int));
  A1_P1 = (int *) A1.ReAlloc(A1_P1, 2 * sizeof(int));
  A1.Free(A1_P1);
  SmallAllocator A2;
  int * A2_P1 = (int *) A2.Alloc(10 * sizeof(int));
  for(unsigned int i = 0; i < 10; i++) A2_P1[i] = i;
  for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 1" << std::endl;
  int * A2_P2 = (int *) A2.Alloc(10 * sizeof(int));
  for(unsigned int i = 0; i < 10; i++) A2_P2[i] = -1;
  for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 2" << std::endl;
  for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 3" << std::endl;
  A2_P1 = (int *) A2.ReAlloc(A2_P1, 20 * sizeof(int));
  for(unsigned int i = 10; i < 20; i++) A2_P1[i] = i;
  for(unsigned int i = 0; i < 20; i++) if(A2_P1[i] != i) std::cout << "ERROR 4" << std::endl;
  for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 5" << std::endl;
  A2_P1 = (int *) A2.ReAlloc(A2_P1, 5 * sizeof(int));
  for(unsigned int i = 0; i < 5; i++) if(A2_P1[i] != i) std::cout << "ERROR 6" << std::endl;
  for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 7" << std::endl;
  A2.Free(A2_P1);
  A2.Free(A2_P2);

  return 0;
}
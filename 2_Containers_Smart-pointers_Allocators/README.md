# Containers. Smart pointers. Allocators.

## Description

Actually, there is only simple tasks for practice before multithreaded programming:
|||
|-------------------|---------------------------------------------------------|
|1. Containers      | Get derivative of polynomial                            |
|2. Smart pointer   | Make your own simple smart pointer                      |
|3. Small allocator | Make your own simple allocator with memory blocks control|


## Result

### Containers

* `std::map` usage to store polynomial 
* `std::regex` for parsing polynomial

##### runtime
![2022-03-12 14-01-55](https://user-images.githubusercontent.com/44144647/158015543-576ab28c-d714-4ca2-b5ee-06484ffc9e3d.gif)


### Smart pointer

Just simple smart pointer with:
* correct constructor
* correct destructor
* allocating memory if pointer is null at access

##### runtime
![2022-03-12 14-07-57](https://user-images.githubusercontent.com/44144647/158015625-215d9f61-f4d8-494e-8e80-c06068815e39.gif)


### Small allocator

Small allocator with:
* own memory block
* memory control in:
  * alloc
  * realloc
  * free
* example without any output

##### runtime
![2022-03-12 14-04-31](https://user-images.githubusercontent.com/44144647/158015573-0791ff92-8ef8-460a-893f-88699f9268ed.gif)

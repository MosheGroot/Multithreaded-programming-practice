# Processes. Signals. Files. Pipes.
## Description

Some simple tasks from the course + additional practice with my own examples:
|||
|-------------------|-----------------------------------------------------------|
|1. [`Processes/1_exec-cat.cpp`](#processes1_exec-catcpp)        | Just execute cat in forked proccess with error checks |
|2. [`Processes/2_Zombie-checker.cpp`](#processes2_zombie-checkercpp)  | Checking zombie child processes in non-blocking mode  |
|3. [`Signals/1_Immortal.cpp`](#signals1_immortalcpp)          | Just making immortal (ignoring `SIGINT` and `SIGTERM`) child process |
|4. [`Signals/2_POSIX.cpp`](#signals2_posixcpp)             | Simple usage of POSIX signals (catching SIGINT) |
|5. [`Files/1_FLock.cpp`](#files1_flockcpp)               | Simple interactive read/write program with `flock` usage |
|6. [`Files/2_LockFile.cpp`](#files2_lockfilecpp)            | Simple program with "lock file" concept usage |
|7. [`Pipes/1_Simple-shell-pipe`](#pipes1_simple-shell-pipe)       | Emulate shell executor with pipe only support |
|8. [`Pipes/2_Fifo`](#pipes2_fifo)                    | Simple sender and client with `FIFO` file usage |




## Result

### `Processes/1_exec-cat.cpp`

Simple exec in forked process (after 1 sec sleep) with reusing argv parameters with.

##### runtime

![Screen Recording 2022-03-17 at 12 09 31 PM](https://user-images.githubusercontent.com/44144647/158776572-3c0f3b56-28a8-492b-af01-83faa5a3a80b.gif)


### `Processes/2_Zombie-checker.cpp`

Fork `CHILDS_NUMBER` processes (with 1 seconds delay between) and make them sleep for `5 + std::rand() % 20` seconds (each process has own random seed).

After fork main process doing `some hard work` and each `WORK_DELAY` seconds calls `zombie_check()` in non-blocking mode.

##### runtime

![Screen Recording 2022-03-17 at 1 52 17 PM](https://user-images.githubusercontent.com/44144647/158794971-b93aeb31-de99-4868-b71f-34829766ef27.gif)


### `Signals/1_Immortal.cpp`

Just making immortal (ignoring `SIGINT` and `SIGTERM`) child process.

##### runtime

![Screen Recording 2022-03-17 at 12 29 11 PM](https://user-images.githubusercontent.com/44144647/158795441-6ad4b09a-ec8f-40c1-8f65-493f366bc1bf.gif)


### `Signals/2_POSIX.cpp`

Catch `SIGINT` with POSIX signal actions.

##### runtime

![Screen Recording 2022-03-17 at 2 35 23 PM](https://user-images.githubusercontent.com/44144647/158800629-0964cb62-1007-437d-84a7-113344e64e40.gif)


### `Files/1_FLock.cpp`

There are two ways to run the program:
* read file (with interactive lock and unlock ability)
* write to the file (with exclusive lock)

In the runtime example, one program runs as reader and another one running as writer:
* Reader sometimes unlocks file to allow writer to write his data.
* Writer waiting for exclusive lock every time he writes

##### runtime

![Screen Recording 2022-03-23 at 4 57 01 PM](https://user-images.githubusercontent.com/44144647/159718182-0c9bb199-d743-4892-88f8-8d7d6d56a877.gif)



### `Files/2_LockFile.cpp`

Simple program with "lock file" concept usage:
* `.lock` file created in some provided work directory every time when program is executed

##### runtime

![Screen Recording 2022-03-23 at 5 33 38 PM](https://user-images.githubusercontent.com/44144647/159724650-076b4756-6ead-4fe9-b3ee-b6f2f9bef4b3.gif)



### `Pipes/1_Simple-shell-pipe`

Read the command line from `stdin`, parsing it to `CommandList` with array of `Command` inside, and execute all of them with pipes between them.

##### runtime

![Screen Recording 2022-03-30 at 1 51 07 PM](https://user-images.githubusercontent.com/44144647/160816730-86195aa7-6203-468f-877f-88d8896b3fc9.gif)



### `Pipes/2_Fifo`

Simple sender and client with `FIFO` file usage.

##### runtime

![Screen Recording 2022-03-30 at 2 03 08 PM](https://user-images.githubusercontent.com/44144647/160821297-55fb5fc2-e567-42ef-803f-85079f5dcc84.gif)



## Learned new
* Non-blocking `waitpid`'s flag (`WNOHANG`)
* ANSI, POSIX and Real Time signals
* Fun fact: files are actually IPC too
* `flock` and "lock file" concept (like in some databases)
* `FIFO` files usage
* Forwarding file descriptors throught socket pair in `Main process <-> Workers` scheme

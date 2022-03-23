# Processes. Signals. Files. Pipes.
## Description

Some simple tasks from the course + additional practice with my own examples:
|||
|-------------------|-----------------------------------------------------------|
|1. `Processes/1_exec-cat.cpp`        | Just execute cat in forked proccess with error checks |
|2. `Processes/2_Zombie-checker.cpp`  | Checking zombie child processes in non-blocking mode  |
|2. `Signals/1_Immortal.cpp`          | Just making immortal (ignoring `SIGINT` and `SIGTERM`) child process |
|2. `Signals/2_POSIX.cpp`             | Simple usage of POSIX signals (catching SIGINT) |
|3. `Files/1_FLock.cpp`               | Simple interactive read/write program with `flock` usage |
|3. `Files/2_LockFile.cpp`            | Simple program with "lock file" concept usage |




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



## Learned new
* Non-blocking `waitpid`'s flag (`WNOHANG`)
* ANSI, POSIX and Real Time signals
* Fun fact: files are actually IPC too
* `flock` and "lock file" concept (like in some databases)

# Processes. Signals. Files. Pipes.
## Description

Some simple tasks from the course + additional practice with my own examples:
|||
|-------------------|-----------------------------------------------------------|
|1. `Processes/1_exec-cat.cpp`        | Just execute cat in forked proccess with error checks |
|2. `Processes/2_Zombie-checker.cpp`  | Checking zombie child processes in non-blocking mode  |
|2. `Signals/1_Immortal.cpp`          | Just making immortal (ignoring `SIGINT` and `SIGTERM`) child process |




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



## Learned new
* Non-blocking `waitpid`'s flag (`WNOHANG`)
* ANSI, POSIX and Real Time signals

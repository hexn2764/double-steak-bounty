# Double Steak Bounty - Developer Page

<p align="center">
  <img src="../docs/images/header.png" alt="Sea Lounge Safe Steak House" />
</p>

This page contains developer instructions for building, running, debugging, and tuning the Double Steak Bounty challenge stack.


## Installation
```bash
chmod +x deploy/* challenge/install_user
cd deploy
./install_dev [-u 8|16|24|32|64] [-s]
```
- `-u` sets the container's internal stack size limit (`ulimit -s`). The default is **8 MiB**.
- `-s` enables GDB attach support (`SYS_PTRACE`). Required for debugging with `gdb -p`.


## Running the Challenge
```bash
docker start vuln-runtime-dev
```
The challenge listens on `localhost:1337` by default.


## Reconfiguration

Solving the challenge requires brute-forcing the **entire** address range where the system may place the stack  (see ../exploit/solution.py). 

If the challenge infrastructure allows it, slightly increasing the allowed stack size does not diminish the learning experience. Doubling the stack size (`ulimit -s`) roughly halves the maximum runtime of the exploit.

If the infrastructure can handle 20 network requests per second, the maximum exploit runtime is approximately:

| Stack Size (MiB)   | Worst-Case Runtime (min)     | Average Runtime (sec) |
|-              |-          |- |
|8          | 12     | 180 |
|16         | 6      | 90|
|24         | 9      | 67|
|32         | 3      | 45|
|64         | 1,5    | 22|

Leaving the stack size at 8 MiB forces challengers to design efficient data structures and search strategies.
Setting it to 64 MiB enables simple linear search approaches, which reduces the educational value of the challenge.

We recommend setting the stack size to 16-24 MiB - this keeps the challenge both engaging and reasonably paced.

For development purposes, setting `-u 64` is practical, as it allows for fast challenge validation.

Reconfigure the stack size with:
```bash
./reconfigure <stack size in MiB>   # Choose from: 8, 16, 24, 32, 64
```
## Infrastructure Reset
To reset the infrastructure and delete all created files, use the cleanup script:
```bash
./cleanup
```

## Exploit Run

After starting the container, navigate to `../exploit/solution.py` and adapt the stack size to match the one chosen during installation:
```python
ULIMIT_S = 64
```
The challenge listens on `localhost:1337`. Run the exploit to retrieve the flag.

For a real-world challenge, replace the `get_flag` script with your preferred flag-handling mechanism.

## Debugging the Challenge Process

Insert a pause at the point of interest in `solution.py`:
```
pause()
```

Run the exploit. Then switch to `root`:
```
su
```

Connect to the running challenge process with GDB:
```
gdb -p $(pidof vuln)
```

**Note**:  If gdb fails to attach, re-run the install script with the -s option to enable GDB support:
```bash
./install_dev -s
```
This starts the container with `SYS_PTRACE` enabled.
See [Docker security](https://docs.docker.com/engine/security/seccomp/) options for details.
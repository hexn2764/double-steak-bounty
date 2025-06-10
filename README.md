# Double Steak Bounty

<p align="center">
  <img src="docs/images/header.png" alt="Sea Lounge Safe Steak House" />
</p>

##   Welcome to the Sea Lounge Safe Steak House! 


**Doubly Steak Bounty** is a CTF challenge that blends several fields of Computer Science, including:
- x86-64 Memory layout
- Algorithms
- Linear algebra
- Probability theory
- Operating systems
- Low-level IT security
- Binary Exploitation

This repository contains everything needed for both **players** and **organizers**, including deployment scripts, challenge binaries, and a full exploit strategy.

## Repository Structure

- `challenge/` - Files provided to CTF players
- `deploy/` - Setup scripts for organizers
- `exploit/` - Full solution and technical write-up


We provide two different challenge views in this repository:
- **User View** -  A CTF player view: test the challenge or try to solve it without getting spoilers. Use this path to challenge yourself and play with the challenge before reading the write-up.
- **Developer View** - A developer path: sets up the full exploit infrastructure, reveals the challenge structure, and contains many spoilers about how to solve the challenge. Use this if you have already played the challenge, want to integrate it into your CTF, or want to test the exploit described in the write-up.

Choose the role you prefer (**User** or **Developer**) and continue with the corresponding installation instructions.




##  Installation Instructions

Before running the challenge, make sure your system is ready:

### 1.  Install Docker

If you don’t have Docker yet, follow the [Docker installation instructions](https://docs.docker.com/engine/install/).

On Ubuntu or Debian, you can also do:

```bash
sudo apt update
sudo apt install -y docker.io
```

---

### 2.  Install required packages

Make sure you have the following tools installed:

```bash
sudo apt update && sudo apt install -y \\
    docker.io \\
    python3 \\
    python3-pip \\
    gcc \\
    gdb \\
    ropgadget
```

---

### 3.  Install Python dependencies

Install `pwntools` and `tqdm`:

```bash
python3 -m pip install --upgrade pip
python3 -m pip install pwntools tqdm
```

If `pip` is missing:

```bash
sudo apt install python3-pip
```

---
### 4.  Install the Challenge

Clone this repository. Choose either **User View** or **Developer View**:

#### 4.1. User View
To test the challenge as if in a CTF, do:
```
cd challenge

chmod +x install_user

./install_user 
```

The installation script creates the challenge infrastructure and populates the `challenge` folder.
After the script is complete, start the challenge with:
```
docker start vuln-runtime-user
```

Then navigate to [challenge/README.md](./challenge/README.md) and start to hack!

#### 4.2 Developer View

Continue with the deployment guide in [deploy/README.md](./deploy/README.md).


## Challenge Write-Up
Enjoy the exploit write-up here: [exploit/README.md](./exploit/README.md). 

To test the exploit, use the Developer installation path.

## License

This project is licensed under the MIT License. See [LICENSE](./LICENSE) for details.  
Attribution is required. If you use or adapt this challenge, please credit the authors:

- Kirill Sedow (GitHub: [hexn2764](https://github.com/hexn2764), contact.kirill@proton.me)
- Lucas Schnack (GitHub: [0Zeta](https://github.com/0Zeta), lucas_schnack@protonmail.com)

# Obsidian
Dark and elegant fits the theme.
**Obsidian** is the only name that fits this scheme

## Prerequisites

Obsc is a **Linux-only** compiler that emits ASM directly.

To use it, you'll need the following tools installed:

- [`g++`](https://man7.org/linux/man-pages/man1/g++.1.html) — to compile the compiler itself
- [`nasm`](https://www.nasm.us/) — to assemble the ASM your `.obs` files produce
- [`ld`](https://man7.org/linux/man-pages/man1/ld.1.html) — to link the object files into an executable

Optional: [`make`](https://man7.org/linux/man-pages/man1/make.1.html) — for convenience; you can also compile everything at once using the commands below.

Most Linux systems can install these via:

```sh
sudo apt install g++ nasm build-essential  # Debian/Ubuntu
sudo dnf install gcc-c++ nasm              # Fedora
sudo pacman -S gcc nasm                    # Arch
```

## Getting Started

``` sh
git clone https://github.com/JustAChillGuy14/Obsidian
cd Obsidian
```

``` sh
make
./main --help # (Note: `--help` is not implemented yet.)
```

If you don't have make:
``` sh
g++ -Wall -Wextra -Wpedantic -Iheaders src/*/*.cpp src/*.cpp -o main # Compiles everything at once,without dependency tracking
```
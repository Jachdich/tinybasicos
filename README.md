# tinybasicos
This is a weird project I'm working on based off of [this](https://github.com/cfenollosa/os-tutorial) tutorial which in turn is based on [this](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf) pdf thing. It's basically just a copy/paste of code so far but I'm going to be working on porting [TinyBASIC](https://github.com/BleuLlama/TinyBasicPlus) to the OS for maximum nostalgia.
# compilation & running
On Linux (macos?) just navigate to the directory in the terminal and type `make` which will create a file called `os-image` which, surprisingly, is the OS image. To run it (tutorial on installing to USB drive etc to come). 
Alternatively, you can use Qemu to emulate this. Either run `make run` or `qemu-system-x86_64 os-image -fda`. 
`make debug` will launch Qemu with GDB as a debugger.

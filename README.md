# CHIP-8

Simple CHIP-8 interpreter written in C++ and SFML.
Interpreter can work in verbose mode where every instruction is decoded and printed to terminal, making debugging much easier.
You can also specify a kill address after reaching which execution will be halted. No longer you need to hit CTRL-C in just the right time.

# Compiling

## Install dependencies:

```shell
sudo apt install build-essential libsfml-dev
```

## Build: 

```shell
make all
```

# Usage
```shell
# Just run the code
./chip8 [filename] 

# Run in verbose mode
./chip8 -v [filename]

# Run in verbose mode with kill address,
#  kill address must be specifiet in decimal format.
./chip8 -v -k [kill_address] [filename]
```

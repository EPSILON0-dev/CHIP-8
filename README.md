# CHIP-8

Simple CHIP-8 interpreter written in C++ and SFML.
Interpreter can work in verbose mode where every instruction is decoded and printed to terminal, making debugging much easier.
You also can run program instruction by instruction for debugging.
You can also specify a kill address after reaching which execution will be halted. No longer you need to hit CTRL-C in just the right time.

# Compiling

### Install dependencies:

```shell
sudo apt install build-essential libsfml-dev
```

### Build: 

```shell
make all
```

### Install:
Don't know why you would want to install it, but just copy the binary to any bin folder, for example:
```shell
cp ./bin/chip8 /usr/bin
```

# Usage
```shell
# Just run the code
chip8 [filename] 

# Run in verbose mode
chip8 -v [filename]

# Run step by step in verbose mode
chip8 -s -v [filename]

# Run in verbose mode with kill address,
#  kill address must be specifiet in decimal format.
chip8 -v -k [kill_address] [filename]
```

# Controls
Everything is binded to left side of the keyboard, so you can use this emulator even of small 60% keyboards.
```
,---,---,---,---,---,---,---,---,---,---,---,---,---,-------,
|   | 7 | 8 | 9 | A |   |   |   |   |   |   |   |   |       |
|---'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-----|
|     | 4 | 5 | 6 | B |   |   |   |   |   |   |   |   |     |
|-----',--',--',--',--',--',--',--',--',--',--',--',--'-----|
|      | 1 | 2 | 3 | C |   |   |   |   |   |   |   |        |
|------'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'--------|
|        | F | 0 | E | D |   |   |   |   |   |   |          |
|----,---',--'--,'---'---'---'---'---'---'-,-'--,'---,------|
|    |    |     |                          |    |    |      |
'----'----'-----'--------------------------'----'----'------'
```
2, 4, 6 and 8 are often used as arrow keys

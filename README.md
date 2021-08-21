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
```
    Usage:
        chip8 [-vsmxcfbk] filename

    Arguments:
        -h          - Show this help
        -v          - Run in verbose mode
        -s          - Run step by step (use space to advance)
        -c value    - Cycles per frame (default: 8)
        -w          - Hide warnings
        -n          - Flip controls
        -x          - Use 8bit color scheme for messages
        -m          - Use monochrome for messages
        -f color    - Set foreground color (HTML format without #)
        -b color    - Set background color (HTML format without #)
        -k address  - Kill address (decimal value)
    
    Examples:
        chip8 test.hex
        chip8 -c 8 -n -f ffffff -b 000000 test.hex
        chip8 -s -v -k 1024 test.hex
```

# Controls
Everything is binded to left side of the keyboard, so you can use this emulator even of small 60% keyboards.
```
,---,---,---,---,---,---,---,---,---,---,---,---,---,-------,
|   |1/7|2/8|3/9| A |   |   |   |   |   |   |   |   |       |
|---'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-----|
|     | 4 | 5 | 6 | B |   |   |   |   |   |   |   |   |     |
|-----',--',--',--',--',--',--',--',--',--',--',--',--'-----|
|      |7/1|8/2|9/3| C |   |   |   |   |   |   |   |        |
|------'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'--------|
|        | F | 0 | E | D |   |   |   |   |   |   |          |
|----,---',--'--,'---'---'---'---'---'---'-,-'--,'---,------|
|    |    |     |                          |    |    |      |
'----'----'-----'--------------------------'----'----'------'
             X/Y - (X - normal), (Y - flipped)
```
2, 4, 6 and 8 are often used as arrow keys

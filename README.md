# Why a Tetris clone?
I embarked on this project as I was inspired by my friend who coded a game in C++ (of significantly greater complexity compared to mine) and wanted to try building something as well. As a start, I chose Tetris as it seemed relatively simple to build. I also took this as an opportunity to learn practical C++.

# Game features
There exists a [great tutorial](https://www.youtube.com/watch?v=wVYKG_ch4yM) on how to code Tetris in C++ using the Raylib library. However, it only covered the basics of the game. For instance, blocks can only be rotated in one direction. I then challenged myself to implement controls and nuances that were not covered in the tutorial. Below is a list that outlines the current and intended future features of my build.

## Gameplay
- [x] Basic movement - Left, right, soft drop (press and hold both supported) and rotations; soft drop only works for low gravity
- [x] Hard drop - Instant drop of tetromino to the bottom for more points and faster combos; instantly locks tetromino
- [x] Next block display - Displays the next block (todo: display multiple possibly)
- [x] Ghost blocks - Representation of where current tetromino will land if allowed to hard drop
- [x] Wall kick - Potentially allows rotation of tetromino when obstructed (cycles through defined cases)
- [x] Levelling system - Increases based on lines cleared; tetromino falls faster with each level
- [x] Lock delay - 0.5 second delay before tetromino is locked
- [ ] Piece holding - To hold pieces for later

## Scoring
- [x] Single
- [x] Double
- [x] Triple
- [x] Tetris
- [x] Soft drop
- [x] Hard drop
- [x] T-spins - mini/regular; no line/single/double/triple
- [ ] Combos
- [ ] Difficult line clears

## Design
- [ ] Music & SFX
- [ ] Custom graphics for blocks
- [ ] Alerts for line clears, t-spins etc

# How to run the project
You will need to install the Raylib library before running this project. 
1. Head to the [Raylib github](https://github.com/raysan5/raylib) and scroll down to build and installation
2. Follow the instructions for whichever OS you are using

Next, you need to ensure that a C++ compiler has been installed on your system (e.g., `g++` for Linux, `MinGW` for Windows or `clang++` for macOS).
- `MinGW` can be downloaded through the [w64devkit](https://github.com/skeeto/w64devkit/releases). Simply follow the instructions and use [Docker](https://www.docker.com/) to build the required file.

To build and run the project, ensure that you are in the `tetris-game` directory.
1. Run the following in the terminal:
```shell
make
```
2. Finally, run the executable (`tetris.exe` on Windows):
```shell
./tetris
```

Happy playing!! 😊​😊​

## Makefile errors
In the event of errors concerning the Makefile, please ensure that the `RAYLIB_PATH` variable correctly defines the path for the installed Raylib library.

# Game states
For easier testing, a handful of game states were generated. They are located as functions in `game.cpp`.

If you like to see/use these cases, simply call the relevant game state function in `Game::Game()`.

# Additional credits
Font used - [Minecraft](https://www.dafont.com/minecraft.font)

#pragma once

#include <vector>
#include <raylib.h>


// Empty cell
extern const Color empty;

// Tetromino colours
extern const Color yellow;
extern const Color cyan;
extern const Color red;
extern const Color green;
extern const Color orange;
extern const Color blue;
extern const Color purple;

// Ghost tetromino colours
extern const Color ghostYellow;
extern const Color ghostCyan;
extern const Color ghostRed;
extern const Color ghostGreen;
extern const Color ghostOrange;
extern const Color ghostBlue;
extern const Color ghostPurple;

// Interface colours
extern const Color darkPurple;
extern const Color lightPurple;
extern const Color darkerPurple;
extern const Color lighterPurple;

std::vector<Color> GetCellColours();
std::vector<Color> GetGhostColours();
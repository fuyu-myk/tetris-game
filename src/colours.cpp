#include <vector>
#include "colours.h"


// Empty cell
const Color empty = {29, 26, 40, 255};

// Tetromino colours
const Color yellow = {237, 234, 4, 255}; // O
const Color cyan = {21, 204, 209, 255}; // I
const Color red = {232, 18, 18, 255}; // Z
const Color green = {47, 230, 23, 255}; // S
const Color orange = {226, 116, 17, 255}; // L
const Color blue = {13, 64, 216, 255}; // J
const Color purple = {166, 0, 247, 255}; // T

// Ghost tetromino colours
const Color ghostYellow = {237, 234, 4, 50}; // O
const Color ghostCyan = {21, 204, 209, 50}; // I
const Color ghostRed = {232, 18, 18, 50}; // Z
const Color ghostGreen = {47, 230, 23, 50}; // S
const Color ghostOrange = {226, 116, 17, 50}; // L
const Color ghostBlue = {13, 64, 216, 50}; // J
const Color ghostPurple = {166, 0, 247, 50}; // T

// Interface colours
const Color darkPurple = {47, 32, 97, 255};
const Color lightPurple = {66, 50, 130, 255};
const Color darkerPurple = {24, 13, 59, 255};
const Color lighterPurple = {90, 72, 163, 255};

/// @brief Fetches colours of tetrominoes.
/// @return Returns a vector of `Color` with an index corresponding to the `id`s of each tetromino.
std::vector<Color> GetCellColours() {
    return {empty, yellow, cyan, red, green, orange, blue, purple};
}

/// @brief Fetches colours of ghost tetrominoes.
/// @return Returns a vector of `Color` with an index corresponding to the `id`s of each tetromino.
std::vector<Color> GetGhostColours() {
    return {empty, ghostYellow, ghostCyan, ghostRed, ghostGreen, ghostOrange, ghostBlue, ghostPurple};
}
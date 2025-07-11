#include "block.h"


// const vector declarations for wall kick tests
const std::vector<std::vector<Position>> defaultClockwise = {
    {Position(0, 0), Position(0, -1), Position(1, -1), Position(-2, 0), Position(-2, -1)},
    {Position(0, 0), Position(0, -1), Position(-1, -1), Position(2, 0), Position(2, -1)},
    {Position(0, 0), Position(0, 1), Position(1, 1), Position(-2, 0), Position(-2, 1)},
    {Position(0, 0), Position(0, 1), Position(-1, 1), Position(2, 0), Position(2, 1)},
};

const std::vector<std::vector<Position>> defaultCounterClockwise = {
    {Position(0, 0), Position(0, 1), Position(1, 1), Position(-2, 0), Position(-2, 1)},
    {Position(0, 0), Position(0, -1), Position(-1, -1), Position(2, 0), Position(2, -1)},
    {Position(0, 0), Position(0, -1), Position(1, -1), Position(-2, 0), Position(-2, -1)},
    {Position(0, 0), Position(0, 1), Position(-1, 1), Position(2, 0), Position(2, 1)},
};

const std::vector<std::vector<Position>> iBlockClockwise = {
    {Position(0, 0), Position(0, 1), Position(0, -2), Position(2, 1), Position(-1, -2)},
    {Position(0, 0), Position(0, -2), Position(0, 1), Position(1, -2), Position(-2, 1)},
    {Position(0, 0), Position(0, -1), Position(0, 2), Position(-2, -1), Position(1, 2)},
    {Position(0, 0), Position(0, 2), Position(0, -1), Position(-1, 2), Position(2, -1)},
};

const std::vector<std::vector<Position>> iBlockCounterClockwise = {
    {Position(0, 0), Position(0, 2), Position(0, -1), Position(-1, 2), Position(2, -1)},
    {Position(0, 0), Position(0, 1), Position(0, -2), Position(2, 1), Position(-1, -2)},
    {Position(0, 0), Position(0, -2), Position(0, 1), Position(1, -2), Position(-2, 1)},
    {Position(0, 0), Position(0, -1), Position(0, 2), Position(-2, -1), Position(1, 2)},
};

const std::vector<Position> oBlock = {
    {Position(0, 0), Position(0, 0), Position(0, 0), Position(0, 0), Position(0, 0)}
};

/// @brief Initialises variables relating to the tetrominoes.
/// @details Inclusive of `cellSize` in pixels, rotation state, colours (both regular and ghost blocks) and offsets.
Block::Block() {
    cellSize = 33;
    rotationState = 0;
    colours = GetCellColours();
    ghostColours = GetGhostColours();
    rowOffset = 0;
    colOffset = 0;
}

/**
 * @brief Draws the block at the specific point on the playboard with the appropriate colour.
 * @param offsetX Offset for column in pixels (default: 181 to account for border and hold).
 * @param offsetY Offset for row in pixels (default: 16 to account for border around playboard).
 */
void Block::Draw(int offsetX, int offsetY) {
    std::vector<Position> tiles = GetCellPositions();

    for (Position item: tiles) {
        DrawRectangle(item.col * cellSize + offsetX, item.row * cellSize + offsetY, cellSize - 1, cellSize - 1, colours[id]);
    }
}

/**
 * @brief Draws the "ghost block" at the specific point on the playboard with the appropriate colour.
 * @details Highlights lowest possible legal position of the tetromino if the player were to "hard drop".
 * Colours are just the original tetromino colours but with decreased opacity.
 * @param ghostRow Offset for row in actual tiles (of the playboard).
 */
void Block::DrawGhost(int ghostRow) {
    std::vector<Position> tiles = GetCellPositions();

    for (Position item: tiles) {
        DrawRectangle(item.col * cellSize + 181, (item.row + ghostRow) * cellSize + 16, cellSize - 1, cellSize - 1, ghostColours[id]);
    }
}

/**
 * @brief Moves tetromino around the playboard.
 * @details Position of the tetromino is calculated by adding/subtracting the offsets to/from
 * the original position as defined in `tetrominoes.cpp`.
 * It is also used to position the tetromino appropriately when it spawns at the top of the playboard.
 * @param rows The row offset.
 * @param cols The column offset.
 */
void Block::Move(int rows, int cols) {
    rowOffset += rows;
    colOffset += cols;
}

/**
 * @brief Queries the position of the tetromino on the playboard.
 * @details The current position is obtained from adding the original defined position in `tetrominoes.cpp`
 * to the row and column offsets.
 * @return A vector of `Position` containing coordinates for each block in the tetromino.
 */
std::vector<Position> Block::GetCellPositions() {
    std::vector<Position> tiles = cells[rotationState];
    std::vector<Position> movedTiles;

    for (Position item: tiles) {
        Position posNew = Position(item.row + rowOffset, item.col + colOffset);
        movedTiles.push_back(posNew);
    }

    return movedTiles;
}

/**
 * @brief Rotates the current tetromino clockwise.
 * @details Handles "wall kicks" through the defined `const` global variables at the top of this document.
 * @return A vector<Position> that handles each "wall kick" case.
 */
std::vector<Position> Block::RotateClockwise() {
    rotationState++;

    if (rotationState >= (int)cells.size()) {
        rotationState = 0;
    }

    switch(id) {
        case 1:
            return oBlock;

        case 2:
            return iBlockClockwise[rotationState];

        default:
            return defaultClockwise[rotationState];
    }
}

/**
 * @brief Rotates the current tetromino counterclockwise.
 * @details Handles "wall kicks" through the defined `const` global variables at the top of this document.
 * @return A vector<Position> that handles each "wall kick" case.
 */
std::vector<Position> Block::RotateCounterClockwise() {
    rotationState--;
    
    if (rotationState < 0) {
        rotationState = (int)cells.size() - 1;
    }

    switch(id) {
        case 1:
            return oBlock;

        case 2:
            return iBlockCounterClockwise[rotationState];

        default:
            return defaultCounterClockwise[rotationState];
    }
}
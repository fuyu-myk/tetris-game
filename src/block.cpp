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

Block::Block() {
    cellSize = 30;
    rotationState = 0;
    colours = GetCellColours();
    ghostColours = GetGhostColours();
    rowOffset = 0;
    colOffset = 0;
}

void Block::Draw(int offsetX, int offsetY) {
    std::vector<Position> tiles = GetCellPositions();

    for (Position item: tiles) {
        DrawRectangle(item.col * cellSize + offsetX, item.row * cellSize + offsetY, cellSize - 1, cellSize - 1, colours[id]);
    }
}

void Block::DrawGhost(int ghostRow) {
    std::vector<Position> tiles = GetCellPositions();

    for (Position item: tiles) {
        DrawRectangle(item.col * cellSize + 11, (item.row + ghostRow) * cellSize + 11, cellSize - 1, cellSize - 1, ghostColours[id]);
    }
}

void Block::Move(int rows, int cols) {
    rowOffset += rows;
    colOffset += cols;
}

std::vector<Position> Block::GetCellPositions() {
    std::vector<Position> tiles = cells[rotationState];
    std::vector<Position> movedTiles;

    for (Position item: tiles) {
        Position posNew = Position(item.row + rowOffset, item.col + colOffset);
        movedTiles.push_back(posNew);
    }

    return movedTiles;
}

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
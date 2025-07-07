#pragma once

#include <vector>
#include <map>
#include "position.h"
#include "colours.h"


class Block {
    public:
        int id;
        std::map<int, std::vector<Position>> cells;
        Block();
        void Draw(int offsetX, int offsetY);
        void DrawGhost(int ghostRow);
        void Move(int rows, int cols);
        std::vector<Position> GetCellPositions();
        std::vector<Position> RotateClockwise();
        std::vector<Position> RotateCounterClockwise();

    private:
        int cellSize;
        int rotationState;
        std::vector<Color> colours;
        std::vector<Color> ghostColours;
        int rowOffset;
        int colOffset;
};
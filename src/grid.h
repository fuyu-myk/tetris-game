#pragma once

#include <vector>
#include <raylib.h>


class Grid {
    public:
        int grid[20][10];

        Grid();
        void Initialise();
        void Print();
        void Draw();
        bool IsOutsideBoundary(int row, int col);
        bool IsCellEmpty(int row, int col);
        int ClearFullRows();

    private:
        int numRows;
        int numCols;
        int cellSize;
        std::vector<Color> colours;
        bool IsRowFull(int row);
        void ClearRow(int row);
        void MoveRowsDown(int row, int numRows);
};
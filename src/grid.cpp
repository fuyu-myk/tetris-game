#include <iostream>
#include "grid.h"
#include "colours.h"

/// @brief Defines grid traits and initialises empty grid and colours.
Grid::Grid() {
    numRows = 20;
    numCols = 10;
    cellSize = 30;

    Initialise();
    colours = GetCellColours();
}

/// @brief Initialises grid array representation to empty, i.e. `0`.
void Grid::Initialise() {
    for(int row = 0; row < numRows; row++) {
        for(int col = 0; col < numCols; col++) {
            grid[row][col] = 0;
        }
    }
}

/// @brief Prints state of the grid array.
void Grid::Print() {
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            std::cout << grid[row][col] << " ";
        }

        std::cout << std::endl;
    }
}

/// @brief Displays current state of the playboard in the game.
void Grid::Draw() {
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            int cellValue = grid[row][col];
            DrawRectangle(col * cellSize + 11, row * cellSize + 11, cellSize - 1, cellSize - 1, colours[cellValue]);
        }
    }
}

/// @brief Checks if the given coordinates are within the defined boundaries.
/// @param row Coordinates for row.
/// @param col Coordinates for column.
/// @return `true` if coordinates are within the defined boundaries, `false` otherwise.
bool Grid::IsOutsideBoundary(int row, int col) {
    if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
        return false;
    }

    return true;
}

/// @brief Checks if the given coordinates contain an empty cell.
/// @param row Coordinates for row.
/// @param col Coordinates for column.
/// @return 'true' if coordinates contain an empty cell, i.e. `grid[row][col] == 0`, `false` otherwise.
bool Grid::IsCellEmpty(int row, int col) {
    if (grid[row][col] == 0) {
        return true;
    }

    return false;
}

/// @brief Clears rows that are full.
/// @details `completed` is used for score calculations.
/// @return Number of rows that are cleared.
int Grid::ClearFullRows() {
    int completed = 0;

    for (int row = numRows - 1; row >= 0; row --) {
        if (IsRowFull(row)) {
            ClearRow(row);
            completed += 1;
        } else if (completed > 0) {
            MoveRowsDown(row, completed);
        }
    }

    return completed;
}

/// @brief Determines if the queried row is full.
/// @param row Queried row.
/// @return `true` if row is full and `false` otherwise.
bool Grid::IsRowFull(int row) {
    for (int col = 0; col < numCols; col ++) {
        if (grid[row][col] == 0) {
            return false;
        }
    }

    return true;
}

/// @brief Resets the target row, i.e. every cell is `0`.
/// @param row Target row.
void Grid::ClearRow(int row) {
    for (int col = 0; col < numCols; col ++) {
        grid[row][col] = 0;
    }
}

/// @brief Moves incomplete rows down.
/// @param row Current row.
/// @param numRows Number of rows to be moved down.
void Grid::MoveRowsDown(int row, int numRows) {
    for (int col = 0; col < numCols; col ++) {
        grid[row + numRows][col] = grid[row][col];
        grid[row][col] = 0;
    }
}
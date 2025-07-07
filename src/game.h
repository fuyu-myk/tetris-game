#pragma once

#include <vector>
#include "grid.h"
#include "tetrominoes.cpp"


class Game {
    public:
        bool gameOver;
        int score;
        Music music;
        Game();
        ~Game();
        void Draw();
        void HandleKeystrokes();
        void MoveDown();
        int HardDrop();
    
    private:
        Grid grid;
        std::vector<Block> blocks;
        Block current;
        Block next;
        Block GetRandomBlock();
        std::vector<Block> GetAllBlocks();
        void MoveLeft();
        void MoveRight();
        void RotateBlockClockwise();
        void RotateBlockCounterClockwise();
        void LockBlock();
        bool IsOutside(int row, int col);
        bool BlockCollision(int row, int col);
        void Reset();
        void UpdateScore(int rowsCleared, int softDropPoints, int hardDropPoints);
        void GhostBlock();
};
#pragma once

#include <vector>
#include "grid.h"
#include "tetrominoes.cpp"


class Game {
    public:
        bool gameOver;
        int score;
        int linesCleared;
        Music music;
        Game();
        ~Game();
        void Draw();
        void HandleSingleKeystrokes();
        void HandleMovementKeystrokes(
            double *leftTime,
            double *rightTime,
            double *downTime,
            double *currentTime,
            bool isGravityStronger
        );
        void MoveDown(bool softDrop);
        int HardDrop();
        void LockDelay();
    
    private:
        Grid grid;
        std::vector<Block> blocks;
        Block current;
        Block next;
        bool lastMoveRotate;
        int lockResets;
        bool lockDelayActive;
        double lockDelayStartTime;
        Block GetRandomBlock();
        std::vector<Block> GetAllBlocks();
        void MoveLeft();
        void MoveRight();
        void RotateBlockClockwise();
        void RotateBlockCounterClockwise();
        bool TSpinType();
        void LockBlock();
        bool IsOutside(int row, int col);
        bool BlockCollision(int row, int col);
        void Reset();
        void UpdateScore(
            int rowsCleared,
            int softDropPoints,
            int hardDropPoints,
            bool tSpinType,
            bool isTSpin
        );
        void GhostBlock();

        // Game States
        void TripleTSpin();
        void DoubleTSpinRegular();
        void DoubleTSpinMini();
        void SingleTSpinRegular();
        void SingleTSpinMini();
};
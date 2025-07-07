#include "block.h"
#include "position.h"


class OBlock: public Block {
    public:
        OBlock() {
            id = 1;

            // Rotation states
            cells[0] = {Position(0, 0), Position(0, 1), Position(1, 0), Position(1, 1)};

            // Centerise
            Move(0, 4);
        }
};

class IBlock: public Block {
    public:
        IBlock() {
            id = 2;

            // Rotation states
            cells[0] = {Position(1, 0), Position(1, 1), Position(1, 2), Position(1, 3)};
            cells[1] = {Position(0, 2), Position(1, 2), Position(2, 2), Position(3, 2)};
            cells[2] = {Position(2, 3), Position(2, 2), Position(2, 1), Position(2, 0)};
            cells[3] = {Position(3, 1), Position(2, 1), Position(1, 1), Position(0, 1)};

            // Centerise
            Move(-1, 3);
        }
};

class SBlock: public Block {
    public:
        SBlock() {
            id = 3;

            // Rotation states
            cells[0] = {Position(1, 0), Position(1, 1), Position(0, 1), Position(0, 2)};
            cells[1] = {Position(0, 1), Position(1, 1), Position(1, 2), Position(2, 2)};
            cells[2] = {Position(1, 2), Position(1, 1), Position(2, 1), Position(2, 0)};
            cells[3] = {Position(2, 1), Position(1, 1), Position(1, 0), Position(0, 0)};

            // Centerise
            Move(0, 3);
        }
};

class ZBlock: public Block {
    public:
        ZBlock() {
            id = 4;

            // Rotation states
            cells[0] = {Position(0, 0), Position(0, 1), Position(1, 1), Position(1, 2)};
            cells[1] = {Position(0, 2), Position(1, 2), Position(1, 1), Position(2, 1)};
            cells[2] = {Position(2, 2), Position(2, 1), Position(1, 1), Position(1, 0)};
            cells[3] = {Position(2, 0), Position(1, 0), Position(1, 1), Position(0, 1)};

            // Centerise
            Move(0, 3);
        }
};

class LBlock: public Block {
    public:
        LBlock() {
            id = 5;

            // Rotation states
            cells[0] = {Position(0, 2), Position(1, 2), Position(1, 1), Position(1, 0)};
            cells[1] = {Position(2, 2), Position(0, 1), Position(1, 1), Position(2, 1)};
            cells[2] = {Position(2, 0), Position(1, 2), Position(1, 1), Position(1, 0)};
            cells[3] = {Position(0, 0), Position(2, 1), Position(1, 1), Position(0, 1)};

            // Centerise
            Move(0, 3);
        }
};

class JBlock: public Block {
    public:
        JBlock() {
            id = 6;

            // Rotation states
            cells[0] = {Position(0, 0), Position(1, 0), Position(1, 1), Position(1, 2)};
            cells[1] = {Position(0, 2), Position(0, 1), Position(1, 1), Position(2, 1)};
            cells[2] = {Position(2, 2), Position(1, 2), Position(1, 1), Position(1, 0)};
            cells[3] = {Position(2, 0), Position(2, 1), Position(1, 1), Position(0, 1)};

            // Centerise
            Move(0, 3);
        }
};

class TBlock: public Block {
    public:
        TBlock() {
            id = 7;

            // Rotation states
            cells[0] = {Position(1, 0), Position(0, 1), Position(1, 2), Position(1, 1)};
            cells[1] = {Position(0, 1), Position(1, 2), Position(2, 1), Position(1, 1)};
            cells[2] = {Position(1, 2), Position(2, 1), Position(1, 0), Position(1, 1)};
            cells[3] = {Position(2, 1), Position(1, 0), Position(0, 1), Position(1, 1)};

            // Centerise
            Move(0, 3);
        }
};
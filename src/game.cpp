#include <random>
#include "game.h"


/// @brief Initialises the game.
/// @details Initialises grid, blocks, score, sound effects and audio, as well as game state.
Game::Game() {
    // Initialising grid and blocks
    grid = Grid();
    blocks = GetAllBlocks();
    current = GetRandomBlock();
    next = GetRandomBlock();

    // Initialising game state and score
    gameOver = false;
    score = 0;

    // Initialising audio
    InitAudioDevice();
    music = LoadMusicStream("assets/music/bgm.mp3");
    PlayMusicStream(music);
}

/// @brief Destructor for the Game class.
/// @details Unloads the music stream and closes the audio device to free resources.
Game::~Game() {
    UnloadMusicStream(music);
    CloseAudioDevice();
}
/**
 * @brief Randomly chooses a block from a vector of all block possibilities.
 * @return The randomly chosen block.
 * @details If the blocks vector is empty, it will refill it with all available blocks.
 * The function uses `arc4random()` to generate a random index to select a block.
 * The selected block is then removed from the vector to ensure it is not chosen again.
 */
Block Game::GetRandomBlock() {
    // When blocks vec is empty
    if (blocks.empty()) {
        blocks = GetAllBlocks();
    }

    int randomIdx = arc4random() % blocks.size();
    Block block = blocks[randomIdx];
    blocks.erase(blocks.begin() + randomIdx);

    return block;
}

/// @brief Generates a vector containing all possible tetrominoes.
/// @return Returns a vector of all possible tetrominoes.
std::vector<Block> Game::GetAllBlocks() {
    return {OBlock(), IBlock(), SBlock(), ZBlock(), LBlock(), JBlock(), TBlock()};
}

/**
 * @brief Draws all objects in the game.
 * @details Aforementioned objects include the game grid, the current block,
 * the next block and the ghost block.
 */
void Game::Draw() {
    grid.Draw();
    current.Draw(11, 11);

    switch(next.id) {
        case 1:
            next.Draw(255, 275);
            break;

        case 2:
            next.Draw(255, 290);
            break;

        default:
            next.Draw(270, 270);
            break;
    }

    GhostBlock();
}

/**
 * @brief Binds keystrokes with game functionality.
 * @details Follows typical tetris keybinds on PC, with keystrokes to
 * move, rotate, hard drop and restart game.
 */
void Game::HandleKeystrokes() {
    int keyStroke = GetKeyPressed();
    if (gameOver && keyStroke != 0) {
        gameOver = false;
        Reset();
    }
    
    switch(keyStroke) {
        case KEY_LEFT:
            MoveLeft();
            break;
        
        case KEY_RIGHT:
            MoveRight();
            break;

        case KEY_DOWN:
            MoveDown();
            UpdateScore(0, 1, 0);
            break;

        case KEY_SPACE: {
            int tilesDropped = HardDrop();
            UpdateScore(0, 0, tilesDropped);
            break;
        }

        case KEY_X:
        case KEY_UP:
            RotateBlockClockwise();
            break;

        case KEY_Z:
        case KEY_LEFT_CONTROL:
            RotateBlockCounterClockwise();
            break;
    }
}

/// @brief Method that houses the "move left" logic.
void Game::MoveLeft() {
    if (!gameOver) {
        current.Move(0, -1);
    
        if (IsOutside(0, 0) || BlockCollision(0, 0)) {
            current.Move(0, 1);
        }
    }
}

/// @brief Method that houses the "move right" logic.
void Game::MoveRight() {
    if (!gameOver) {
        current.Move(0, 1);
    
        if (IsOutside(0, 0) || BlockCollision(0, 0)) {
            current.Move(0, -1);
        }
    }
}

/// @brief Method that houses the "move down" or "soft drop" logic.
void Game::MoveDown() {
    if (!gameOver) {
        current.Move(1, 0);
    
        if (IsOutside(0, 0) || BlockCollision(0, 0)) {
            current.Move(-1, 0);
            LockBlock();
        }
    }
}

/// @brief Method that houses the "hard drop" logic.
/// @return Returns the number of cells travelled.
/// @note The number of cells travelled is used to calculate the score for hard drops.
int Game::HardDrop() {
    int tilesDropped = 0;

    if (!gameOver) {
        while (!IsOutside(0, 0) && !BlockCollision(0, 0)) {
            current.Move(1, 0);
            tilesDropped++;
        }

        current.Move(-1, 0);
        LockBlock();
    }

    return tilesDropped - 1;
}

/// @brief Method that houses the "clockwise rotation" logic.
/// @details Checks if tetromino passes any of the "Wall Kick" tests.
/// If a test passes, the block will be placed in the appropriate position.
/// If all tests fail, the block will not rotate
void Game::RotateBlockClockwise() {
    if (!gameOver) {
        // Wall kick cases for rotation
        std::vector<Position> wallKickCases = current.RotateClockwise();
    
        bool rotated = false;
        if (IsOutside(0, 0) || BlockCollision(0, 0)) {
            for (int i = 0; i < 5; i++) {
                if (!IsOutside(wallKickCases[i].row, wallKickCases[i].col) && 
                    !BlockCollision(wallKickCases[i].row, wallKickCases[i].col)) {
                    current.Move(wallKickCases[i].row, wallKickCases[i].col);
                    rotated = true;
                    // play rotate sound
                    break;
                }
    
            }

            // Test failed: Undo rotation
            if (!rotated) {
                current.RotateCounterClockwise();
            }
        } else {
            // play rotate sound
        }
    }
}

/// @brief Method that houses the "counterclockwise rotation" logic.
/// @details Checks if tetromino passes any of the "Wall Kick" tests.
/// If a test passes, the block will be placed in the appropriate position.
/// If all tests fail, the block will not rotate.
void Game::RotateBlockCounterClockwise() {
    if (!gameOver) {
        // Wall kick cases for rotation
        std::vector<Position> wallKickCases = current.RotateCounterClockwise();
    
        bool rotated = false;
        if (IsOutside(0, 0) || BlockCollision(0, 0)) {
            for (int i = 0; i < 5; i++) {
                if (!IsOutside(wallKickCases[i].row, wallKickCases[i].col) && 
                    !BlockCollision(wallKickCases[i].row, wallKickCases[i].col)) {
                    current.Move(wallKickCases[i].row, wallKickCases[i].col);
                    rotated = true;
                    // play rotate sound
                    break;
                }
            }

            // Test failed: Undo rotation
            if (!rotated) {
                current.RotateCounterClockwise();
            }
        } else {
            // play rotate sound
        }
    }
}

/// @brief Method that prevents current block from being moved.
/// @note If a collision occurs, i.e. a new block is generated in another, the game ends.
/// 
/// This method is also used to determine how many rows are cleared via `.ClearFullRows()`
/// and updates the score accordingly.
void Game::LockBlock() {
    std::vector<Position> tiles = current.GetCellPositions();

    for (Position item: tiles) {
        grid.grid[item.row][item.col] = current.id;
    }

    current = next;
    if (BlockCollision(0, 0)) {
        gameOver = true;
    }

    next = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    UpdateScore(rowsCleared, 0, 0);
}
/**
 * @brief Method to determine if the tetromino is moved outside the playable grid.
 * @param row Modifier to current tile row.
 * @param col Modifier to current tile column.
 * @return Returns `true` if the tetromino is outside the boundary and `false` otherwise.
 */
bool Game::IsOutside(int row, int col) {
    std::vector<Position> tiles = current.GetCellPositions();

    for (Position item: tiles) {
        if (grid.IsOutsideBoundary(item.row + row, item.col + col)) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Checks if a block collides with another.
 * @param row Modifier to current tile row.
 * @param col Modifier to current tile column.
 * @return Returns `true` if there is a collision and `false` otherwise.
 */
bool Game::BlockCollision(int row, int col) {
    std::vector<Position> tiles = current.GetCellPositions();

    for (Position item: tiles) {
        if (!grid.IsCellEmpty(item.row + row, item.col + col)) {
            return true;
        }
    }

    return false;
}

/// @brief Resets the game state.
/// @details Used to launch a new game when the game is over.
void Game::Reset() {
    grid.Initialise();
    blocks = GetAllBlocks();
    current = GetRandomBlock();
    next = GetRandomBlock();
    score = 0;
}

/// @brief Method that houses the ghost block logic
/// @details Ghost blocks aid the player to determine the lowest possible position of a tetromino
/// on the grid before it is locked.
void Game::GhostBlock() {
    std::vector<Position> tiles = current.GetCellPositions();
    int ghostRow = 0;
    bool canDrop = true;
    
    while (canDrop) {
        for (Position item: tiles) {
            int testRow = item.row + ghostRow + 1;
            if (!grid.IsCellEmpty(testRow, item.col) ||
            grid.IsOutsideBoundary(testRow, item.col)) {
                canDrop = false;
                break;
            }
        }
        
        if (canDrop) {
            ghostRow++;
        }
    }
    
    // Draw ghost block
    current.DrawGhost(ghostRow);
}

/**
 * @brief Updates the score with reference to the common tetris scoring system.
 * @details Score updates are calculated and subsequently added to the public `score` variable.
 * @param rowsCleared Number of rows cleared in a single `.LockBlock()` call.
 * @param softDropPoints Number of tiles moved to calculate the score for "Soft Drop".
 * @param hardDropPoints Number of tiles moved to calculate the score for "Hard Drop".
 */
void Game::UpdateScore(int rowsCleared, int softDropPoints, int hardDropPoints) {
    // Handling line clears
    switch (rowsCleared) {
        case 1:
            score += 100;
            break;

        case 2:
            score += 300;
            break;

        case 3:
            score += 500;
            break;

        case 4:
            score += 800;
            break;

        default:
            break;
    }

    // Handling drops
    score += softDropPoints;
    score += hardDropPoints * 2;
}
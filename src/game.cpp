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
    hold.id = 0;

    // Game state
    // Load a game state from one of the game state functions
    //TripleTSpin();
    
    // Initialising game attributes and score
    gameOver = false;
    lastMoveRotate = false;
    score = 0;
    linesCleared = 0;
    lockResets = 15;
    lockDelayActive = false;
    lockDelayStartTime = 0.0;
    justHeld = false;
    comboCount = -1;
    b2bDifficult = false;

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
 * The function uses `rand()` to generate a random index to select a block.
 * The selected block is then removed from the vector to ensure repeated block spawns are impossible.
 */
Block Game::GetRandomBlock() {
    // Reset max lock resets
    lockResets = 15;

    // When blocks vec is empty
    if (blocks.empty()) {
        blocks = GetAllBlocks();
    }

    int randomIdx = rand() % blocks.size();
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
    DrawRectangle(173, 8, 346, 676, lighterPurple);
    DrawRectangle(181, 16, 330, 660, darkPurple);
    grid.Draw();
    current.Draw(181, 16);

    switch(next.id) {
        case 1:
            next.Draw(387 + 50, 48 + 50);
            break;

        case 2:
            next.Draw(420 + 17, 48 + 65);
            break;

        default:
            next.Draw(420 + 33, 48 + 49);
            break;
    }

    switch(hold.id) {
        case 0:
            break;

        case 1:
            OBlock().Draw(-123 + 50, 48 + 50);
            break;

        case 2:
            IBlock().Draw(-91 + 17, 48 + 65);
            break;

        case 3:
            SBlock().Draw(-91 + 33, 48 + 49);
            break;

        case 4:
            ZBlock().Draw(-91 + 33, 48 + 49);
            break;

        case 5:
            LBlock().Draw(-91 + 33, 48 + 49);
            break;

        case 6:
            JBlock().Draw(-91 + 33, 48 + 49);
            break;

        case 7:
            TBlock().Draw(-91 + 33, 48 + 49);
            break;
    }

    GhostBlock();
}

/**
 * @brief Binds non-movement keystrokes with game functionality.
 * @details Follows typical tetris keybinds on PC, with keystrokes to
 * rotate, hard drop and restart game.
 */
void Game::HandleSingleKeystrokes() {
    int keyStroke = GetKeyPressed();
    if (gameOver && keyStroke != 0) {
        gameOver = false;
        Reset();
    } else {
        switch(keyStroke) {
            case KEY_SPACE: {
                int tilesDropped = HardDrop();
                UpdateScore(0, 0, tilesDropped, false, false);
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
            
            case KEY_C:
            case KEY_LEFT_SHIFT:
                HoldBlock();
                break;
        }
    }
}

/**
 * @brief Binds movement keystrokes with game functionality.
 * @details Follows typical tetris keybinds on PC for movement.
 * Press once to move one tile.
 * Hold to move across multiple tiles at a constant rate.
 * @param leftTime Pointer to `lastMoveLeftTime` in `main.cpp`.
 * @param rightTime Pointer to `lastMoveRightTime` in `main.cpp`.
 * @param downTime Pointer to `lastMoveDownTime` in `main.cpp`.
 * @param currentTime Pointer to `currentTime` in `main.cpp`.
 */
void Game::HandleMovementKeystrokes(
    double *leftTime, double *rightTime, double *downTime, double *currentTime, bool isGravityStronger
) {
    const double moveInterval = 0.1;

    if (IsKeyPressed(KEY_LEFT)) {
        MoveLeft();
        *leftTime = *currentTime;
    } else if (IsKeyDown(KEY_LEFT) && *currentTime - *leftTime >= moveInterval) {
        MoveLeft();
        *leftTime = *currentTime;
    }

    if (IsKeyPressed(KEY_RIGHT)) {
        MoveRight();
        *rightTime = *currentTime;
    } else if (IsKeyDown(KEY_RIGHT) && *currentTime - *rightTime >= moveInterval) {
        MoveRight();
        *rightTime = *currentTime;
    }

    if (IsKeyPressed(KEY_DOWN) && !isGravityStronger) {
        MoveDown(true);
        *downTime = *currentTime;
    } else if ((IsKeyDown(KEY_DOWN) && *currentTime - *downTime >= moveInterval) && !isGravityStronger) {
        MoveDown(true);
        *downTime = *currentTime;
    }
}

/// @brief Method that houses the "move left" logic.
/// @details If tetromino is under lock delay, decrement `lockResets` and reset delay time.
void Game::MoveLeft() {
    if (!gameOver) {
        lastMoveRotate = false;
        current.Move(0, -1);
    
        if (IsOutside(0, 0) || BlockCollision(0, 0)) {
            current.Move(0, 1);
            return;
        }

        if (lockDelayActive) {
            lockDelayStartTime = GetTime();
            lockResets -= 1;
        }
    }
}

/// @brief Method that houses the "move right" logic.
/// @details If tetromino is under lock delay, decrement `lockResets` and reset delay time.
void Game::MoveRight() {
    if (!gameOver) {
        lastMoveRotate = false;
        current.Move(0, 1);
    
        if (IsOutside(0, 0) || BlockCollision(0, 0)) {
            current.Move(0, -1);
            return;
        }

        if (lockDelayActive) {
            lockDelayStartTime = GetTime();
            lockResets -= 1;
        }
    }
}

/// @brief Method that houses the "move down" or "soft drop" logic.
void Game::MoveDown(bool softDrop) {
    if (!gameOver) {
        if (IsOutside(1, 0) || BlockCollision(1, 0)) {
            // Block cannot move another tile down
            // If last move was a rotate, it should still be true

            // Start lock block timer
            if (!lockDelayActive) {
                lockDelayActive = true;
                lockDelayStartTime = GetTime();
            }
        } else {
            // Block can still be in free-fall
            // If last move was a rotate, it will not be true after this move
            current.Move(1, 0);
            lastMoveRotate = false;

            if (softDrop) {
                UpdateScore(0, 1, 0, false, false);
            }

            lockDelayActive = false;
        }
    }
}

/// @brief Method that houses the "hard drop" logic.
/// @return Returns the number of cells travelled.
/// @note The number of cells travelled is used to calculate the score for hard drops.
int Game::HardDrop() {
    int tilesDropped = 0;

    if (!gameOver) {
        lastMoveRotate = false;
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
///
/// If tetromino is under lock delay, decrement `lockResets` and reset delay time.
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
                    lastMoveRotate = true;

                    if (lockDelayActive) {
                        lockDelayStartTime = GetTime();
                        lockResets -= 1;
                    }

                    // play rotate sound
                    break;
                }
            }

            // Test failed: Undo rotation
            if (!rotated) {
                current.RotateCounterClockwise();
                lastMoveRotate = false;
            }
        } else {
            lastMoveRotate = true;

            if (lockDelayActive) {
                lockDelayStartTime = GetTime();
                lockResets -= 1;
            }

            // play rotate sound
        }
    }
}

/// @brief Method that houses the "counterclockwise rotation" logic.
/// @details Checks if tetromino passes any of the "Wall Kick" tests.
/// If a test passes, the block will be placed in the appropriate position.
/// If all tests fail, the block will not rotate.
///
/// If tetromino is under lock delay, decrement `lockResets` and reset delay time.
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
                    lastMoveRotate = true;

                    if (lockDelayActive) {
                        lockDelayStartTime = GetTime();
                        lockResets -= 1;
                    }

                    // play rotate sound
                    break;
                }
            }

            // Test failed: Undo rotation
            if (!rotated) {
                current.RotateCounterClockwise();
                lastMoveRotate = false;
            }
        } else {
            lastMoveRotate = true;

            if (lockDelayActive) {
                lockDelayStartTime = GetTime();
                lockResets -= 1;
            }

            // play rotate sound
        }
    }
}

/**
 * @brief Determines whether the T-Spin is a mini T-Spin or a regular T-Spin.
 * @details This function is only called if `current.id` corresponds to a T-Block, i.e. `current.id == 7` and when `lastMoveRotate` is `true`.
 * All tetrominoes are created with an invisible grid in mind. Using this property, it is easy to find the center block
 * of the T-Block in the `TBlock` class under `tetrominoes.cpp`. See the `note` for more information.
 * @return `true` if it is a regular T-Spin, `false` if it is a mini T-Spin.
 * @note The `centerBlock` is obtained through `tiles[3]` because it is index of the center block defined in all rotations of the T-Block.
 * 
 * See `class TBlock` in `tetrominoes.cpp`.
 */
bool Game::TSpinType() {
    std::vector<Position> tiles = current.GetCellPositions();
    Position centerBlock = tiles[3];
    std::vector<Position> cornerBlocks = {Position(-1, -1), Position(-1, 1), Position(1, 1), Position(1, -1)};
    int noCornersFilled = 0;

    printf("Center: (%d, %d)\n", centerBlock.row, centerBlock.col);
    for (Position corner: cornerBlocks) {
        int checkRow = centerBlock.row + corner.row;
        int checkCol = centerBlock.col + corner.col;

        printf("Checking corner (%d, %d) ", checkRow, checkCol);
        if (!grid.IsOutsideBoundary(checkRow, checkCol) &&
            grid.grid[checkRow][checkCol] != 0) {
            printf("FILLED\n");
            noCornersFilled += 1;
        } else {
            printf("EMPTY/OUTSIDE\n");
        }

        if (noCornersFilled >= 3) {
            printf("Corners filled: %d\n", noCornersFilled);
            printf("Regular T-Spin\n");
            return true;
        }
    }
    printf("Corners filled: %d\n", noCornersFilled);
    printf("T-Spin mini\n");
    return false;
}

/// @brief Method that prevents current block from being moved.
/// @note If a collision occurs, i.e. a new block is generated in another, the game ends.
/// 
/// This method is also used to determine how many rows are cleared via `.ClearFullRows()`
/// and updates the score accordingly.
void Game::LockBlock() {
    std::vector<Position> tiles = current.GetCellPositions();
    bool tSpinType = false;
    bool isTSpin = false;

    for (Position item: tiles) {
        grid.grid[item.row][item.col] = current.id;
    }
    
    if (lastMoveRotate == true && current.id == 7) {
        tSpinType = TSpinType();
        isTSpin = true;
    }

    current = next;
    if (BlockCollision(0, 0)) {
        gameOver = true;
    }

    next = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    if (rowsCleared > 0) {
        comboCount++;
    } else {
        comboCount = -1;
    }
    
    UpdateScore(rowsCleared, 0, 0, tSpinType, isTSpin);
    lastMoveRotate = false;
    linesCleared += rowsCleared;
    justHeld = false;
}

/// @brief Method that is called in the game loop for lock delay.
/// @details Maximum time before a tetromino is locked is 0.5 seconds.
/// Timer is reset if tetromino is in free fall again or moved/rotated.
/// Maximum number of moves/rotations (when not in free fall) is 15.
void Game::LockDelay() {
    const double maxDelay = 0.5;
    
    if (lockDelayActive) {
        if (IsOutside(1, 0) || BlockCollision(1, 0)) {
            if ((GetTime() - lockDelayStartTime) >= maxDelay || lockResets <= 0) {
                LockBlock();
                lockDelayActive = false;
                lockResets = 15;
            }
        } else {
            lockDelayActive = false;
        }
    }
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
    lastMoveRotate = false;
    linesCleared = 0;
    lockResets = 15;
    lockDelayActive = false;
    lockDelayStartTime = 0.0;
    justHeld = false;
    comboCount = -1;
    b2bDifficult = false;
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

void Game::HoldBlock() {
    if (!justHeld) {
        justHeld = true;
        current.rotationState = 0;

        if (hold.id == 0) {
            hold = current;
            current = next;
            next = GetRandomBlock();
        } else {
            Block temp = hold;
            hold = current;

            switch (temp.id) {
                case 1:
                    current = OBlock();
                    break;

                case 2:
                    current = IBlock();
                    break;

                case 3:
                    current = SBlock();
                    break;

                case 4:
                    current = ZBlock();
                    break;

                case 5:
                    current = LBlock();
                    break;

                case 6:
                    current = JBlock();
                    break;

                case 7:
                    current = TBlock();
                    break;
            }
        }
    }
}

/**
 * @brief Updates the score with reference to the common tetris scoring system.
 * @details Score updates are calculated and subsequently added to the public `score` variable.
 * Each relevant case has checks for T-Spins in order to award the appropriate scores.
 * @param rowsCleared Number of rows cleared during a single `.LockBlock()` call.
 * @param softDropPoints Number of tiles moved to calculate the score for "Soft Drop".
 * @param hardDropPoints Number of tiles moved to calculate the score for "Hard Drop".
 * @param tSpinType Type of T-Spin: `true` for regular, `false` for mini.
 * @param isTSpin Whether the last move before locking is a T-Spin.
 */
void Game::UpdateScore(int rowsCleared, int softDropPoints, int hardDropPoints, bool tSpinType, bool isTSpin) {
    int calcLevel = 1 + floor(linesCleared / 10);
    int level = (calcLevel <= 15) ? calcLevel : 15;

    // Handling line clears
    switch (rowsCleared) {
        case 1: {
            if (isTSpin) {
                if (tSpinType) {
                    score += (b2bDifficult) ? 800 * level * 1.5 : 800 * level;
                    b2bDifficult = true;
                    break;
                } else {
                    score += (b2bDifficult) ? 200 * level * 1.5 : 200 * level;
                    b2bDifficult = true;
                    break;
                }
            } else {
                score += 100 * level;
                b2bDifficult = false;
                break;
            }
        }

        case 2: {
            if (isTSpin) {
                if (tSpinType) {
                    score += (b2bDifficult) ? 1200 * level * 1.5 : 1200 * level;
                    b2bDifficult = true;
                    break;
                } else {
                    score += (b2bDifficult) ? 400 * level * 1.5 : 400 * level;
                    b2bDifficult = true;
                    break;
                }
            } else {
                score += 300 * level;
                b2bDifficult = false;
                break;
            }
        }

        case 3: {
            if (isTSpin) {
                score += (b2bDifficult) ? 1600 * level * 1.5 : 1600 * level;
                b2bDifficult = true;
                break;
            } else {
                score += 500 * level;
                b2bDifficult = false;
                break;
            }
        }

        case 4:
            score += (b2bDifficult) ? 800 * level * 1.5 : 800 * level;
            b2bDifficult = true;
            break;

        default: {
            if (isTSpin) {
                if (tSpinType) {
                    score += 400 * level;
                    break;
                } else {
                    score += 100 * level;
                    break;
                }
            } else {
                break;
            }
        }
    }

    // Handling drops
    score += softDropPoints;
    score += hardDropPoints * 2;

    // Handling combos
    if (comboCount >= 0) {
        score += (comboCount * 50) * level;
    }
}


/// @brief Renders a Triple T-Spin setup on the playboard
void Game::TripleTSpin() {
    grid.grid[18][0] = 6;
    grid.grid[19][0] = 6;
    grid.grid[19][1] = 6;
    grid.grid[19][2] = 6;

    grid.grid[18][1] = 7;
    grid.grid[17][1] = 7;
    grid.grid[17][2] = 7;
    grid.grid[17][0] = 7;

    grid.grid[19][4] = 1;
    grid.grid[19][5] = 1;
    grid.grid[18][4] = 1;
    grid.grid[18][5] = 1;

    grid.grid[19][6] = 2;
    grid.grid[19][7] = 2;
    grid.grid[19][8] = 2;
    grid.grid[19][9] = 2;

    grid.grid[17][5] = 4;
    grid.grid[17][6] = 4;
    grid.grid[18][6] = 4;
    grid.grid[18][7] = 4;

    grid.grid[18][8] = 3;
    grid.grid[17][8] = 3;
    grid.grid[17][7] = 3;
    grid.grid[16][7] = 3;
    
    grid.grid[18][9] = 5;
    grid.grid[17][9] = 5;
    grid.grid[16][9] = 5;
    grid.grid[16][8] = 5;

    grid.grid[17][4] = 5;
    grid.grid[16][4] = 5;
    grid.grid[15][4] = 5;
    grid.grid[15][3] = 5;
}

/// @brief Renders a regular Double T-Spin setup on the playboard
void Game::DoubleTSpinRegular() {
    grid.grid[18][0] = 6;
    grid.grid[19][0] = 6;
    grid.grid[19][1] = 6;
    grid.grid[19][2] = 6;

    grid.grid[18][1] = 4;
    grid.grid[17][1] = 4;
    grid.grid[17][2] = 4;
    grid.grid[16][2] = 4;

    grid.grid[19][4] = 2;
    grid.grid[19][5] = 2;
    grid.grid[19][6] = 2;
    grid.grid[19][7] = 2;

    grid.grid[18][5] = 5;
    grid.grid[18][6] = 5;
    grid.grid[18][7] = 5;
    grid.grid[17][7] = 5;

    grid.grid[18][8] = 1;
    grid.grid[18][9] = 1;
    grid.grid[19][8] = 1;
    grid.grid[19][9] = 1;
}

/// @brief Renders a mini Double T-Spin setup on the playboard
void Game::DoubleTSpinMini() {
    grid.grid[18][0] = 6;
    grid.grid[19][0] = 6;
    grid.grid[19][1] = 6;
    grid.grid[19][2] = 6;

    grid.grid[18][1] = 2;
    grid.grid[17][1] = 2;
    grid.grid[16][1] = 2;
    grid.grid[15][1] = 2;

    grid.grid[19][4] = 2;
    grid.grid[19][5] = 2;
    grid.grid[19][6] = 2;
    grid.grid[19][7] = 2;

    grid.grid[18][5] = 5;
    grid.grid[18][6] = 5;
    grid.grid[18][7] = 5;
    grid.grid[17][7] = 5;

    grid.grid[18][8] = 1;
    grid.grid[18][9] = 1;
    grid.grid[19][8] = 1;
    grid.grid[19][9] = 1;
}

/// @brief Renders a regular Single T-Spin setup on the playbord
void Game::SingleTSpinRegular() {
    grid.grid[17][2] = 4;
    grid.grid[17][3] = 4;
    grid.grid[18][3] = 4;
    grid.grid[18][4] = 4;

    grid.grid[17][5] = 1;
    grid.grid[17][6] = 1;
    grid.grid[18][5] = 1;
    grid.grid[18][6] = 1;

    grid.grid[17][7] = 6;
    grid.grid[18][7] = 6;
    grid.grid[18][8] = 6;
    grid.grid[18][9] = 6;

    grid.grid[19][0] = 2;
    grid.grid[19][1] = 2;
    grid.grid[19][2] = 2;
    grid.grid[19][3] = 2;

    grid.grid[19][6] = 2;
    grid.grid[19][7] = 2;
    grid.grid[19][8] = 2;
    grid.grid[19][9] = 2;
}

/// @brief Renders a mini Single T-Spin setup on the playboard
void Game::SingleTSpinMini() {
    grid.grid[18][2] = 4;
    grid.grid[18][3] = 4;
    grid.grid[19][3] = 4;
    grid.grid[19][4] = 4;

    grid.grid[18][5] = 1;
    grid.grid[18][6] = 1;
    grid.grid[19][5] = 1;
    grid.grid[19][6] = 1;

    grid.grid[18][7] = 6;
    grid.grid[19][7] = 6;
    grid.grid[19][8] = 6;
    grid.grid[19][9] = 6;
}
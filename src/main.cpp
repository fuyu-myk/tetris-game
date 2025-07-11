#include <iostream>
#include <math.h>
#include <raylib.h>
#include "game.h"
#include "colours.h"


double gravity_lastUpdateTime = 0;
double interval = 0.8;
double lastMoveLeftTime = 0;
double lastMoveRightTime = 0;
double lastMoveDownTime = 0;

/**
 * @brief Checks if the specified interval has passed since the last update.
 * @details This function compares the current time with the last update time and returns true if it has elapsed.
 * If the interval has passed, it updates the last update time to the current time.
 * @param interval The time interval in seconds to check against.
 * @return `true` if the interval has passed, `false` otherwise.
 * @note This is used to control the automatic falling of tetrominoes in the game.
 */
bool EventTrigger(double interval, double *lastUpdateTime) {
    double currentTime = GetTime();

    if (currentTime - gravity_lastUpdateTime >= interval) {
        *lastUpdateTime = currentTime;
        return true;
    }

    return false;
}

/// @brief Handles the gravity of the tetromino based on the level of the player.
/// @param level Level of the player.
/// @return Interval before tetromino falls one tile in seconds.
double Gravity(int level) {
    int x = level - 1;

    return pow(0.8 - (x * 0.007), x);
}

int main() {
    // Initialising game window & attributes
    InitWindow(692, 756, "Tetris");
    SetTargetFPS(60);

    Font font = LoadFontEx("fonts/Minecraft.ttf", 64, 0, 0);

    // Creating game instance
    Game game = Game();

    // Game loop
    while (WindowShouldClose() == false) {
        UpdateMusicStream(game.music);
        game.HandleSingleKeystrokes();

        // Tetromino movement
        double currentTime = GetTime();
        bool isGravityStronger = interval < 0.1;
        game.HandleMovementKeystrokes(&lastMoveLeftTime, &lastMoveRightTime, &lastMoveDownTime, &currentTime, isGravityStronger);

        // Gravity - Pauses when moving down; resumes once not moving down
        bool downKeyUp = IsKeyUp(KEY_DOWN);

        if (downKeyUp || isGravityStronger) {
            int calcLevel = 1 + floor(game.linesCleared / 10);
            int level = (calcLevel <= 15) ? calcLevel : 15;
            double* ptrInterval = &interval;
            *ptrInterval = Gravity(level);

            if (EventTrigger(interval, &gravity_lastUpdateTime)) {
                game.MoveDown(false);
            }
        }

        // Lock delay
        game.LockDelay();

        // Drawing
        BeginDrawing();
        ClearBackground(darkPurple);

        // Score
        DrawRectangle(0, 676, 692, 80, darkerPurple);
        DrawRectangle(0, 676, 692, 8, lighterPurple);
        char scoreText[10];
        snprintf(scoreText, sizeof(scoreText), "%d", game.score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 35, 2);

        DrawTextEx(font, scoreText, {181 + (330 - textSize.x) / 2, 692}, 35, 2, WHITE);

        // Next block
        DrawRectangleRounded({511, 8, 181, 213}, 0.3, 6, lighterPurple);
        DrawRectangle(511, 8, 90, 8, lighterPurple);
        DrawTextEx(font, "Next", {519 + 33, 16}, 30, 10, WHITE);
        DrawRectangleRounded({519, 48, 165, 165}, 0.3, 6, darkerPurple);

        // Hold block
        DrawRectangleRounded({0, 8, 181, 213}, 0.3, 6, lighterPurple);
        DrawRectangle(91, 8, 90, 8, lighterPurple);
        DrawTextEx(font, "Hold", {8 + 37, 16}, 30, 10, WHITE);
        DrawRectangleRounded({8, 48, 165, 165}, 0.3, 6, darkerPurple);
        
        game.Draw();
        
        // Game over
        if (game.gameOver) {
            DrawRectangle(0, 0, 692, 676, {0, 0, 0, 150});
            DrawTextEx(font, "Game Over", {197, 274}, 50, 2, WHITE);
            DrawTextEx(font, "Press any key", {267, 324}, 20, 2, WHITE);
            DrawTextEx(font, "to restart", {267 + 25, 341}, 20, 2, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
}
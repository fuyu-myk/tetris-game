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
    InitWindow(500, 620, "Tetris");
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
        DrawTextEx(font, "Score", {352, 15}, 35, 2, WHITE);
        DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, lightPurple);

        char scoreText[10];
        snprintf(scoreText, sizeof(scoreText), "%d", game.score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 35, 2);

        DrawTextEx(font, scoreText, {320 + (170 - textSize.x) / 2, 70}, 35, 2, WHITE);

        // Next block
        DrawTextEx(font, "Next", {363, 175}, 35, 2, WHITE);
        DrawRectangleRounded({320, 215, 170, 180}, 0.3, 6, lightPurple);
        
        // Game over
        if (game.gameOver) {
            DrawTextEx(font, "Game Over", {320, 450}, 27, 2, WHITE);
            DrawTextEx(font, "Press any key", {325, 500}, 20, 2, WHITE);
            DrawTextEx(font, "to restart", {350, 525}, 20, 2, WHITE);
        }

        game.Draw();

        EndDrawing();
    }

    CloseWindow();
}
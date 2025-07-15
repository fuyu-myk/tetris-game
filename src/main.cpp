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
int lastRecordedLinesCleared = 0;
double reportStartTime = -1; // -1 means no active report
bool hasActiveReport = false;
int reportLinesCleared = 0;
bool reportTSpinRegular = false;
bool reportTSpinMini = false;
bool reportB2B = false;
bool lastTSpinRegular = false;
bool lastTSpinMini = false;

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

void Report(int numLinesCleared, bool tSpinRegular, bool tSpinMini, bool b2b, Font font) {
    if (tSpinRegular) {
        DrawTextEx(font, "T-Spin", {8 + 71, 590}, 24, 2, WHITE);
    } else if (tSpinMini) {
        DrawTextEx(font, "Mini", {8 + 116, 564}, 20, 2, WHITE);
        DrawTextEx(font, "T-Spin", {8 + 71, 590}, 24, 2, WHITE);
    }

    if (numLinesCleared > 0) {
        switch(numLinesCleared) {
            case 1:
                DrawTextEx(font, "SINGLE", {8 + 34, 620}, 30, 2, WHITE);
                break;

            case 2:
                DrawTextEx(font, "DOUBLE", {8 + 20, 620}, 30, 2, WHITE);
                break;

            case 3:
                DrawTextEx(font, "TRIPLE", {8 + 30, 620}, 30, 2, WHITE);
                break;

            case 4:
                DrawTextEx(font, "TETRIS", {8 + 30, 620}, 30, 2, WHITE);
                break;
            
            default:
                break;
        }
    }

    if (b2b) {
        DrawTextEx(font, "b2b x1.5", {8 + 82, 652}, 14, 2, WHITE);
    }
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

        // Combo count
        if (game.comboCount > 0) {
            char comboText[10];
            snprintf(comboText, sizeof(comboText), "%d COMBO", game.comboCount);
            Vector2 comboSize = MeasureTextEx(font, comboText, 24, 2);
            DrawTextEx(font, comboText, {8 + (165 - comboSize.x) / 2, 240}, 24, 2, WHITE);
        }

        // Reporting - Check for new events to report
        int currentLinesCleared = game.linesCleared - lastRecordedLinesCleared;
        bool newTSpinRegular = game.tSpinRegular && !lastTSpinRegular;
        bool newTSpinMini = game.tSpinMini && !lastTSpinMini;
        bool hasNewReport = (newTSpinMini || newTSpinRegular || currentLinesCleared > 0);
        
        // If we have a new report, start it immediately (replacing any existing one)
        if (hasNewReport) {
            hasActiveReport = true;
            reportStartTime = currentTime;
            reportLinesCleared = currentLinesCleared;
            reportTSpinRegular = newTSpinRegular;
            reportTSpinMini = newTSpinMini;
            reportB2B = game.b2b;
            lastRecordedLinesCleared = game.linesCleared;
        }
        
        // Update T-spin state tracking
        lastTSpinRegular = game.tSpinRegular;
        lastTSpinMini = game.tSpinMini;
        
        // Display active report if within 3 second window
        if (hasActiveReport && (currentTime - reportStartTime < 3.0)) {
            Report(reportLinesCleared, reportTSpinRegular, reportTSpinMini, reportB2B, font);
        } else if (hasActiveReport) {
            // Report has expired, clear it
            hasActiveReport = false;
        }
        
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
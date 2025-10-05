// Author: James Sutter
// Date: 18 Oct 2025
// Language: C++
// Description: Create a simple version of Pong using C++
#pragma once
#include "Ball.h"
#include "Paddle.h"
#include <windows.h>
#include <vector>

enum class GameMode { Menu, OnePlayer, Normal };

class Game
{
public:
    Paddle player1, player2;
    Ball ball;
    int score1, score2;
    GameMode mode;

    // Menu selection: 0 = 1 Player, 1 = 2 Players
    int menuSelection;

    Game();

    void Update();
    void Draw(HDC hdc);
    void DrawMenu(HDC hdc);
    void HandleInput(bool up1, bool down1, bool up2, bool down2);
    void ResetBall();
    void UpdateAI();
};
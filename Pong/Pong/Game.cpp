#include "Game.h"
#include <string>
#include <cmath>
#include <cstdlib>

Game::Game() :
    player1(50, 250, 20, 100),
    player2(730, 250, 20, 100),
    ball(400, 300, 15),
    score1(0), score2(0),
    mode(GameMode::Menu),
    menuSelection(0)
{
    ResetBall();
}

void Game::ResetBall()
{
    ball.x = 400;
    ball.y = 300;

    // Ball 2x faster than original
    ball.speedX = (rand() % 2 == 0 ? 10.0f : -10.0f); // original 5 -> 10
    ball.speedY = (rand() % 2 == 0 ? 4.0f : -4.0f);   // original 2 -> 4
}

void Game::Update()
{
    if (mode == GameMode::Menu)
    {
        // Animate ball in menu
        ball.Move();
        return;
    }

    // Move ball
    ball.Move();

    // Bounce off top/bottom
    if (ball.y - ball.radius < 0)
    {
        ball.y = ball.radius;
        ball.speedY = -ball.speedY;
    }
    if (ball.y + ball.radius > 600)
    {
        ball.y = 600 - ball.radius;
        ball.speedY = -ball.speedY;
    }

    // Paddle collisions
    auto HandlePaddle = [&](Paddle& paddle, bool isPlayer1)
        {
            if (ball.x + ball.radius > paddle.x && ball.x - ball.radius < paddle.x + paddle.width &&
                ball.y + ball.radius > paddle.y && ball.y - ball.radius < paddle.y + paddle.height)
            {
                // Position ball outside paddle
                if (isPlayer1)
                    ball.x = paddle.x + paddle.width + ball.radius;
                else
                    ball.x = paddle.x - ball.radius;

                // Calculate vertical impact
                float relativeY = (ball.y - (paddle.y + paddle.height / 2.0f)) / (paddle.height / 2.0f);
                float bounceAngle = relativeY * (3.14159f / 4.0f); // max 45 deg

                float speed = std::sqrt(ball.speedX * ball.speedX + ball.speedY * ball.speedY);
                speed *= 1.05f; // 5% faster after hitting paddle

                ball.speedX = (isPlayer1 ? 1 : -1) * fabs(speed * std::cos(bounceAngle));
                ball.speedY = speed * -std::sin(bounceAngle);

                // Prevent stuck horizontal movement
                if (fabs(ball.speedY) < 2.0f)
                    ball.speedY = (ball.speedY < 0 ? -2.0f : 2.0f);
            }
        };

    HandlePaddle(player1, true);
    HandlePaddle(player2, false);

    // Scoring
    if (ball.x - ball.radius < 0)
    {
        score2++;
        ResetBall();
    }
    if (ball.x + ball.radius > 800)
    {
        score1++;
        ResetBall();
    }

    // Win condition
    if (score1 >= 8 || score2 >= 8)
    {
        mode = GameMode::Menu;
        score1 = score2 = 0;
        ResetBall();
    }

    // AI for player 2
    if (mode == GameMode::OnePlayer)
    {
        int paddleCenter = player2.y + player2.height / 2;
        int delta = (ball.y - paddleCenter) / 3;
        if (delta > 5) delta = 5;
        if (delta < -5) delta = -5;
        player2.y += delta;

        // Clamp
        if (player2.y < 0) player2.y = 0;
        if (player2.y + player2.height > 600) player2.y = 600 - player2.height;
    }
}

void Game::Draw(HDC hdc)
{
    // Draw dashed middle line
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    for (int y = 0; y < 600; y += 20)
    {
        MoveToEx(hdc, 400, y, NULL);
        LineTo(hdc, 400, y + 10);
    }
    SelectObject(hdc, oldPen);
    DeleteObject(pen);

    if (mode == GameMode::Menu)
    {
        DrawMenu(hdc);
        return;
    }

    player1.Draw(hdc);
    player2.Draw(hdc);
    ball.Draw(hdc);

    // Draw scores
    std::wstring s1 = std::to_wstring(score1);
    std::wstring s2 = std::to_wstring(score2);
    TextOut(hdc, 350, 20, s1.c_str(), s1.length());
    TextOut(hdc, 420, 20, s2.c_str(), s2.length());
}

void Game::DrawMenu(HDC hdc)
{
    player1.Draw(hdc);
    player2.Draw(hdc);
    ball.Draw(hdc);

    HFONT hFont = CreateFont(72, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        VARIABLE_PITCH, TEXT("Arial"));
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, 300, 50, L"PONG", 4);
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);

    std::vector<std::wstring> menu = { L"1 Player", L"2 Players" };
    for (int i = 0; i < menu.size(); i++)
    {
        int y = 200 + i * 40;
        std::wstring text = (i == menuSelection ? L"> " : L"  ") + menu[i];
        TextOut(hdc, 300, y, text.c_str(), text.length());
    }
}

void Game::HandleInput(bool up1, bool down1, bool up2, bool down2)
{
    if (up1) player1.MoveUp();
    if (down1) player1.MoveDown();

    if (mode == GameMode::Normal)
    {
        if (up2) player2.MoveUp();
        if (down2) player2.MoveDown();
    }

    // Clamp paddles
    if (player1.y < 0) player1.y = 0;
    if (player1.y + player1.height > 600) player1.y = 600 - player1.height;
    if (player2.y < 0) player2.y = 0;
    if (player2.y + player2.height > 600) player2.y = 600 - player2.height;
}
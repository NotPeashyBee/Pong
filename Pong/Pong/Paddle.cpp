// Author: James Sutter
// Date: 18 Oct 2025
// Language: C++
// Description: Create a simple version of Pong using C++
#include "Paddle.h"

Paddle::Paddle(int nx, int ny, int w, int h) : x(nx), y(ny), width(w), height(h), speed(7) {}

void Paddle::MoveUp()
{
    y -= speed;
    if (y < 0) y = 0;
}

void Paddle::MoveDown()
{
    y += speed;
    if (y + height > 600) y = 600 - height;
}

void Paddle::Draw(HDC hdc)
{
    RECT rect = { x,y,x + width,y + height };
    FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
}
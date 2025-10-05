// Ball.cpp
#include "Ball.h"
#include <cstdlib>
#include <cmath>

Ball::Ball(int nx, int ny, int r) : x(nx), y(ny), radius(r)
{
    Reset(nx, ny);
}

void Ball::Move()
{
    x += speedX;
    y += speedY;

    // Bounce off top and bottom edges
    if (y - radius < 0)
    {
        y = radius;
        speedY = -speedY;
    }
    if (y + radius > 600)
    {
        y = 600 - radius;
        speedY = -speedY;
    }
}

void Ball::Draw(HDC hdc)
{
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
}

void Ball::Reset(int nx, int ny)
{
    x = nx;
    y = ny;

    // Randomize horizontal direction
    speedX = (rand() % 2 == 0 ? 5.0f : -5.0f);

    // Randomize vertical speed slightly
    speedY = ((rand() % 3) - 1) * 2.0f; // -2, 0, or 2
}
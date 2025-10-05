// Author: James Sutter
// Date: 18 Oct 2025
// Language: C++
// Description: Create a simple version of Pong using C++
#pragma once
#include <windows.h>

class Ball
{
public:
    int x, y, radius;
    int speedX, speedY;
    Ball(int x, int y, int r);
    void Move();
    void Draw(HDC hdc);
    void Reset(int nx, int ny);
};
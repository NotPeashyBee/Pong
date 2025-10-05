// Author: James Sutter
// Date: 1 Oct 2025
// Language: C++
// Description: Create a simple version of Pong using C++
#pragma once
#include <windows.h>

class Paddle
{
public:
    int x, y, width, height;
    int speed;
    Paddle(int nx, int ny, int w, int h);
    void MoveUp();
    void MoveDown();
    void Draw(HDC hdc);
};
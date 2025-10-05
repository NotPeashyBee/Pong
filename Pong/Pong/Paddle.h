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
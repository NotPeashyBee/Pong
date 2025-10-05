// Ball.h
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
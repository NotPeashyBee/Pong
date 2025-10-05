#include <Windows.h>
#include "Game.h"
#include <string>

// Forward declaration
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Game object
Game game;

// Input flags for paddles
bool up1 = false, down1 = false;
bool up2 = false, down2 = false;

// Window size
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;

// Double buffer
HBITMAP hBitmap = nullptr;
HDC hMemDC = nullptr;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    // 1. Register window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"PongClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    // 2. Create window
    HWND hwnd = CreateWindowEx(
        0,
        L"PongClass",
        L"Pong Game",
        WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),
        CW_USEDEFAULT, CW_USEDEFAULT,
        WIN_WIDTH, WIN_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nCmdShow);

    // 3. Prepare double buffer
    HDC hdc = GetDC(hwnd);
    hMemDC = CreateCompatibleDC(hdc);
    hBitmap = CreateCompatibleBitmap(hdc, WIN_WIDTH, WIN_HEIGHT);
    SelectObject(hMemDC, hBitmap);

    // 4. Main loop
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Update input and game state
        game.HandleInput(up1, down1, up2, down2);
        game.Update();

        // Clear screen
        HBRUSH bg = CreateSolidBrush(RGB(0, 0, 0));
        RECT rect = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
        FillRect(hMemDC, &rect, bg);
        DeleteObject(bg);

        // Draw game or menu
        game.Draw(hMemDC);

        // Draw "PONG" title on menu
        if (game.mode == GameMode::Menu)
        {
            HFONT hFont = CreateFont(72, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                VARIABLE_PITCH, TEXT("Arial"));
            HFONT hOldFont = (HFONT)SelectObject(hMemDC, hFont);
            SetTextColor(hMemDC, RGB(255, 255, 255));
            SetBkMode(hMemDC, TRANSPARENT);
            TextOut(hMemDC, 300, 50, L"PONG", 4);
            SelectObject(hMemDC, hOldFont);
            DeleteObject(hFont);
        }

        // Present buffer to screen
        BitBlt(hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT, hMemDC, 0, 0, SRCCOPY);

        Sleep(16); // ~60 FPS
    }

    // Cleanup
    DeleteDC(hMemDC);
    DeleteObject(hBitmap);
    ReleaseDC(hwnd, hdc);

    return 0;
}

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_KEYDOWN:
        if (game.mode == GameMode::Menu)
        {
            switch (wParam)
            {
            case VK_UP: if (game.menuSelection > 0) game.menuSelection--; break;
            case VK_DOWN: if (game.menuSelection < 1) game.menuSelection++; break; // only 2 options now
            case VK_RETURN:
                switch (game.menuSelection)
                {
                case 0: game.mode = GameMode::OnePlayer; break;
                case 1: game.mode = GameMode::Normal; break;
                }
                game.ResetBall();
                break;
            }
        }
        else
        {
            // Paddle controls
            switch (wParam)
            {
            case 'W': up1 = true; break;
            case 'S': down1 = true; break;
            case VK_UP: up2 = true; break;
            case VK_DOWN: down2 = true; break;
            case VK_ESCAPE: PostQuitMessage(0); break;
            }
        }
        break;

    case WM_KEYUP:
        switch (wParam)
        {
        case 'W': up1 = false; break;
        case 'S': down1 = false; break;
        case VK_UP: up2 = false; break;
        case VK_DOWN: down2 = false; break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
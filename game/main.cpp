#include <iostream>
#include <windows.h>
#include <wingdi.h>

int exit() { std::exit(1); return 1; }

int main() {
    HWND consoleHandle = GetConsoleWindow();
    HDC deviceContext = GetDC(consoleHandle);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
    GetConsoleScreenBufferInfo(hOut, &scrBufferInfo);

    short winWidth = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1;
    short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;
    short scrBufferWidth = scrBufferInfo.dwSize.X;
    short scrBufferHeight = scrBufferInfo.dwSize.Y;
    COORD newSize;
    newSize.X = scrBufferWidth;
    newSize.Y = winHeight;
    int Status = SetConsoleScreenBufferSize(hOut, newSize);
    if (! Status) std::cout << "Failure\n" << exit();


    HPEN pen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
    
    std::string b;
    while (true) {
        SelectObject(deviceContext, pen);
        LineTo(deviceContext, 300, 300);
        LineTo(deviceContext, 300, 400);
        LineTo(deviceContext, 0, 0);
        DrawMenuBar(consoleHandle);
        std::cin >> b;
    }

    ReleaseDC(consoleHandle, deviceContext);
    
    getchar();
}
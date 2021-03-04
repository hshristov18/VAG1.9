#include <windows.h>
#include <winuser.h>
#include <wingdi.h>
#include <commctrl.h>
#include "Data.h"

const HBRUSH bgColor = CreateSolidBrush(RGB(200, 200, 200));

void AddMenus(HWND hwnd) {

    HWND textField, button;
    HMENU hMenubar;
    HMENU hMenu;

    hMenubar = CreateMenu();
    hMenu = CreateMenu();

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&File");
    SetMenu(hwnd, hMenubar);

    button = CreateWindow("BUTTON", "HUIIII", WS_VISIBLE | WS_CHILD | WS_BORDER,
        20, 30, 150, 20, hwnd, NULL, NULL, NULL);
}
//function used for making other elements on the window
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam) {

    switch (msg) {


    case WM_CREATE:

        AddMenus(hwnd);
        break;

    case WM_COMMAND:

    case WM_DESTROY:

        PostQuitMessage(0);
        break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

//Main function
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR lpCmdLine, int nCmdShow) {

    MSG  msg;
    WNDCLASSW wc = { 0 };
    wc.lpszClassName = L"Simple menu";
    wc.hInstance = hInstance;
    wc.hbrBackground = bgColor;
    wc.lpfnWndProc = WindowProcedure;
    wc.hCursor = LoadCursor(0, IDC_ARROW);

    //loading the window
    RegisterClassW(&wc);
    CreateWindowW(wc.lpszClassName, L"Title",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        500, 200, 550, 550, 0, 0, hInstance, 0);

    while (GetMessage(&msg, NULL, 0, 0)) {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
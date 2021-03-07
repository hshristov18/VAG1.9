#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include <stdio.h>
#include <vector>
#include "Resource.h"
using namespace std;
const wchar_t g_szClassName[] = TEXT("myWindowClass");
HINSTANCE hInst;

INT_PTR CALLBACK AboutDlgProc(HWND hDlg,
    UINT uMessage,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (uMessage)
    {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        switch (wParam)
        {
        case IDOK:
            EndDialog(hDlg, IDOK);
            break;

        case IDCANCEL:
            EndDialog(hDlg, IDOK);
            break;
        }
        return TRUE;
    }

    return FALSE;
}

void greetings(const HDC& hdc)
{
    TextOutA(hdc, 10, 10, "Hello, User!!", 14);
    TextOutA(hdc, 10, 30, "Wellcome the our program", 25);
    TextOutA(hdc, 10, 50, "Choose an option by clicking the button.", 41);
}

void createButtons(vector<HWND>& hButtons,HWND hwnd)
{
    HWND temp;
    temp=CreateWindowA("BUTTON", "Show data", WS_VISIBLE | WS_CHILD,
        40, 70, 100, 20, hwnd, (HMENU)IDB_SHOW_DATA, hInst, NULL);
    hButtons.push_back(temp);
    temp = CreateWindowA("BUTTON", "Edit data", WS_VISIBLE | WS_CHILD,
        40, 100, 100, 20, hwnd, (HMENU)IDB_EDIT_DATA, hInst, NULL);
    hButtons.push_back(temp);
    temp = CreateWindowA("BUTTON", "Exit", WS_VISIBLE | WS_CHILD,
        40, 130, 100, 20, hwnd, IDB_EXIT, hInst, NULL);
    hButtons.push_back(temp);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    vector<HWND> hButtons;
    switch (msg)
    {
    case WM_PAINT:
         hdc = BeginPaint(hwnd, &ps); 
         greetings(hdc);
        EndPaint(hwnd, &ps);
    
    break;
    case WM_CREATE:
    {
        createButtons(hButtons,hwnd);

        HMENU hMenu, hSubMenu, hSubMenuAbout;
        HANDLE hIcon, hIconSm;

        hMenu = CreateMenu();
        hSubMenu = CreatePopupMenu();
        hSubMenuAbout = CreatePopupMenu();
        AppendMenu(hSubMenu, MF_STRING, IDM_NEW, TEXT("&New"));
        AppendMenu(hSubMenu, MF_STRING, IDM_EXIT, TEXT("E&xit"));
        AppendMenu(hMenu, MF_STRING|MF_POPUP, (UINT)hSubMenu,
            TEXT("&File"));
        AppendMenu(hSubMenuAbout, MF_STRING, IDM_ABOUT, TEXT("A&bout.."));
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenuAbout,
            TEXT("&Help"));
        hSubMenu = CreatePopupMenu();
        
        SetMenu(hwnd, hMenu);       

        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDB_EXIT:PostQuitMessage(0); break;
        case IDB_SHOW_DATA:MessageBox(hwnd, TEXT("DEV NEEDS TIME"),
            TEXT("File"), MB_OK); break;
        case IDB_EDIT_DATA:MessageBox(hwnd, TEXT("DEV NEEDS COFFEE"),
            TEXT("File"), MB_OK); break;
        }
        switch (GET_WM_COMMAND_ID(wParam,lParam))
        {
        case IDM_EXIT:
            PostQuitMessage(0);
            break;
        case IDM_NEW:
            MessageBox(hwnd, TEXT("DEV NEEDS SLEEP"), TEXT("File") , MB_OK);
            break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
            break;
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    hInst = hInstance;
    //Step 1: Registering the Window Class
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = MAKEINTRESOURCE(IDM_MAIN_MENU);
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBoxA(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        TEXT("The title of window"),
        WS_OVERLAPPEDWINDOW,
        450, 250, 550, 550,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBoxA(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
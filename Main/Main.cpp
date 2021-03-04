#pragma warning(disable : 4996)
#include <windows.h>
#include <winuser.h>
#include <wingdi.h>
#include <commctrl.h>
#include <uiautomationcore.h>
#include <tchar.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <libloaderapi.h>

using namespace std;

const HBRUSH bgColor = CreateSolidBrush(RGB(200, 200, 200));
HINSTANCE hInst;

int ID_HEADER;

// DoInsertItem - inserts an item into a header control. 
// Returns the index of the new item. 
// hwndHeader - handle to the header control. 
// iInsertAfter - index of the previous item. 
// nWidth - width of the new item. 
// lpsz - address of the item string. 
int DoInsertItem(HWND hwndHeader, int iInsertAfter,
	int nWidth, LPTSTR lpsz)
{
	HDITEM hdi;
	int index;

	hdi.mask = HDI_TEXT | HDI_FORMAT | HDI_WIDTH;
	hdi.cxy = nWidth;
	hdi.pszText = lpsz;
	hdi.cchTextMax = sizeof(hdi.pszText) / sizeof(hdi.pszText[0]);
	hdi.fmt = HDF_LEFT | HDF_STRING;

	index = SendMessage(hwndHeader, HDM_INSERTITEM,
		(WPARAM)iInsertAfter, (LPARAM)&hdi);

	return index;
}


HWND DoCreateHeader(HWND hwndParent, HDC hdc)
{
	HWND hwndHeader;
	RECT rcParent;
	HDLAYOUT hdl;
	WINDOWPOS wp;

	vector <string> indexes =
	{
		"Forename",
		"Middlename",
		"Surname",
		"Class"
	};
	int posOfItem = 0;

	// Ensure that the common control DLL is loaded, and then create 
	// the header control. 
	INITCOMMONCONTROLSEX icex;  //declare an INITCOMMONCONTROLSEX Structure
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;   //set dwICC member to ICC_LISTVIEW_CLASSES    
										 // this loads list-view and header control classes.


	if ((hwndHeader = CreateWindowEx(0, WC_HEADER, (LPCTSTR)NULL,
		WS_CHILD | WS_BORDER | HDS_BUTTONS | HDS_HORZ,
		0, 0, 0, 0, hwndParent, (HMENU)ID_HEADER, hInst,
		(LPVOID)NULL)) == NULL)
		return (HWND)NULL;

	// Retrieve the bounding rectangle of the parent window's 
	// client area, and then request size and position values 
	// from the header control. 
	GetClientRect(hwndParent, &rcParent);

	hdl.prc = &rcParent;
	hdl.pwpos = &wp;
	if (!SendMessage(hwndHeader, HDM_LAYOUT, 0, (LPARAM)&hdl))
		return (HWND)NULL;

	// Set the size, position, and visibility of the header control. 
	SetWindowPos(hwndHeader, wp.hwndInsertAfter, wp.x, wp.y,
		wp.cx, wp.cy, wp.flags | SWP_SHOWWINDOW);
	char* cstr = new char[indexes[0].length() + 1];
	for (int i = 0; i < indexes.size(); i++)
	{
		cstr = new char[indexes[i].length() + 1];

		strcpy(cstr, indexes[i].c_str());

		posOfItem = DoInsertItem(hwndHeader, posOfItem, 95, LPTSTR(cstr));
		posOfItem++;
	}
	delete[] cstr;
	
	return hwndHeader;
}

void AddMenus(HWND hwnd) {


	HMENU hMenubar;
	HMENU hMenu;
	HWND button;
	hMenubar = CreateMenu();
	hMenu = CreateMenu();

	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&File");
	SetMenu(hwnd, hMenubar);

	button = CreateWindow(
		LPCSTR("BUTTON"),  // Predefined class; Unicode assumed 
		LPCSTR("OK"),      // Button text 
		WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
		700,         // x position 
		100,         // y position 
		30,        // Button width
		30,        // Button height
		hwnd,     // Parent window
		NULL,// No menu.
		NULL,
		NULL);

}



//function used for making other elements on the window
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {


	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, User!");
	hdc = BeginPaint(hwnd, &ps);

	// Here your application is laid out.
	// For this introduction, we just print out "Hello, Windows desktop!"
	// in the top left corner.
	TextOut(hdc,
		40, 40,
		greeting, _tcslen(greeting));
	// End application specific layout section.

	EndPaint(hwnd, &ps);
	switch (msg)
	{
		case WM_CREATE: {
			AddMenus(hwnd); break;
			HWND hListView = CreateWindow(
				WC_LISTVIEW, "", WS_CHILD | LVS_REPORT | LVS_EDITLABELS,
				20, 20,
				300, 100,
				hwnd, (HMENU)100, 0, NULL);
			if (0 == hListView)
			{
				MessageBox(hwnd, "Error List View!!!", "ERROR!", 0);
			}

			
		}
		case WM_COMMAND: DoCreateHeader(hwnd, hdc); break;

		case WM_DESTROY: PostQuitMessage(0); break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


//Main function
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PWSTR lpCmdLine, int nCmdShow) {

	static TCHAR szWindowClass[] = _T("DesktopApp");
	static TCHAR szTitle[] = _T("Title");

	MSG  msg;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowProcedure;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = bgColor;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	// Store instance handle in our global variable
	hInst = hInstance;

	//loading the window
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		500, 200,
		750, 650,
		0,
		0,
		hInstance,
		0
	);
	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	BOOL bRet;

	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}
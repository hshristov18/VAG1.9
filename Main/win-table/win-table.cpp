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
#include <windowsx.h>
#include <stdio.h>
#pragma comment(lib, "comctl32.lib")

#include "Resource.h"

const char ClassName[] = "MainWindowClass";
const char ChildClassName[] = "ChildWindowClass";
TCHAR       g_ClassName[] = TEXT("VListVwClass");
HWND hWndClient;
HINSTANCE   hInst;
#define ID_LISTVIEW  2000
#define ITEM_COUNT   100
#define ErrorHandler() ErrorHandlerEx(__LINE__, __FILE__)

LRESULT CALLBACK ChildProc(HWND    hWnd,

	UINT    Msg,

	WPARAM  wParam,

	LPARAM  lParam)
{

	switch (Msg)

	{
		// Handle messages from the child windows
	}

	return DefMDIChildProc(hWnd, Msg, wParam, lParam);

}
void ErrorHandlerEx(WORD wLine, LPSTR lpszFile)
{
	LPSTR lpvMessage;
	DWORD  dwError;
	TCHAR  szBuffer[256];

	// Allow FormatMessage() to look up the error code returned by GetLastError
	dwError = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		(LPTSTR)&lpvMessage,
		0,
		NULL);

	// Check to see if an error occurred calling FormatMessage()
	if (0 == dwError)
	{
		_sntprintf_s(szBuffer, _countof(szBuffer), _TRUNCATE,
			TEXT("An error occurred calling FormatMessage().")
			TEXT("Error Code %d"),
			GetLastError());
		MessageBox(NULL,
			szBuffer,
			TEXT("Generic"),
			MB_ICONSTOP | MB_ICONEXCLAMATION);
		return;
	}

	// Display the error information along with the place the error happened.
	_sntprintf_s(szBuffer, _countof(szBuffer), _TRUNCATE,
		TEXT("Generic, Line=%d, File=%s"), wLine, lpszFile);
	MessageBox(NULL, lpvMessage, szBuffer, MB_ICONEXCLAMATION | MB_OK);
}

void ResizeListView(HWND hwndListView, HWND hwndParent)
{
	RECT  rc;

	GetClientRect(hwndParent, &rc);

	MoveWindow(hwndListView,
		rc.left,
		rc.top,
		rc.right - rc.left,
		rc.bottom - rc.top,
		TRUE);

	//only call this if we want the LVS_NOSCROLL style
	//PositionHeader(hwndListView);
}

void PositionHeader(HWND hwndListView)
{
	HWND  hwndHeader = GetWindow(hwndListView, GW_CHILD);
	DWORD dwStyle = GetWindowLong(hwndListView, GWL_STYLE);

	/*To ensure that the first item will be visible, create the control without
	the LVS_NOSCROLL style and then add it here*/
	dwStyle |= LVS_NOSCROLL;
	SetWindowLong(hwndListView, GWL_STYLE, dwStyle);

	//only do this if we are in report view and were able to get the header hWnd
	if (((dwStyle & LVS_TYPEMASK) == LVS_REPORT) && hwndHeader)
	{
		RECT        rc;
		HD_LAYOUT   hdLayout;
		WINDOWPOS   wpos;

		GetClientRect(hwndListView, &rc);
		hdLayout.prc = &rc;
		hdLayout.pwpos = &wpos;

		Header_Layout(hwndHeader, &hdLayout);

		SetWindowPos(hwndHeader,
			wpos.hwndInsertAfter,
			wpos.x,
			wpos.y,
			wpos.cx,
			wpos.cy,
			wpos.flags | SWP_SHOWWINDOW);

		ListView_EnsureVisible(hwndListView, 0, FALSE);
	}
}

BOOL InsertListViewItems(HWND hwndListView)
{
	//empty the list
	ListView_DeleteAllItems(hwndListView);

	//set the number of items in the list
	ListView_SetItemCount(hwndListView, ITEM_COUNT);

	return TRUE;
}

BOOL InitListView(HWND hwndListView)
{
	LV_COLUMN   lvColumn;
	int         i;
	TCHAR       szString[5][20] = { TEXT("Main Column"), TEXT("Column 1"), TEXT("Column 2"), TEXT("Column 3"), TEXT("Column 4") };

	//empty the list
	ListView_DeleteAllItems(hwndListView);

	//initialize the columns
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 120;
	for (i = 0; i < 5; i++)
	{
		lvColumn.pszText = szString[i];
		ListView_InsertColumn(hwndListView, i, &lvColumn);
	}

	InsertListViewItems(hwndListView);

	return TRUE;
}



void UpdateMenu(HWND hwndListView, HMENU hMenu)
{
	UINT  uID = IDM_LIST_CHILD;
	DWORD dwStyle;

	//uncheck all of these guys
	CheckMenuItem(hMenu, IDM_LARGE_ICONS_CHILD, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, IDM_SMALL_ICONS_CHILD, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, IDM_LIST_CHILD, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, IDM_REPORT, MF_BYCOMMAND | MF_UNCHECKED);

	//check the appropriate view menu item
	dwStyle = GetWindowLong(hwndListView, GWL_STYLE);
	switch (dwStyle & LVS_TYPEMASK)
	{
	case LVS_ICON:
		uID = IDM_LARGE_ICONS_CHILD;
		break;

	case LVS_SMALLICON:
		uID = IDM_SMALL_ICONS_CHILD;
		break;

	case LVS_LIST:
		uID = IDM_LIST_CHILD;
		break;

	case LVS_REPORT:
		uID = IDM_REPORT;
		break;
	}
	CheckMenuRadioItem(hMenu, IDM_LARGE_ICONS_CHILD, IDM_REPORT, uID, MF_BYCOMMAND | MF_CHECKED);
}

LRESULT ListViewNotify(HWND hWnd, LPARAM lParam)
{
	LPNMHDR  lpnmh = (LPNMHDR)lParam;
	HWND     hwndListView = GetDlgItem(hWnd, ID_LISTVIEW);

	switch (lpnmh->code)
	{
	case LVN_GETDISPINFO:
	{
		LV_DISPINFO* lpdi = (LV_DISPINFO*)lParam;
		TCHAR szString[MAX_PATH];

		if (lpdi->item.iSubItem)
		{
			if (lpdi->item.mask & LVIF_TEXT)
			{
				_sntprintf_s(szString, _countof(szString), _TRUNCATE,
					TEXT("Item %d - Column %d"),
					lpdi->item.iItem + 1, lpdi->item.iSubItem);
				_tcsncpy_s(lpdi->item.pszText, lpdi->item.cchTextMax,
					szString, _TRUNCATE);
			}
		}
		else
		{
			if (lpdi->item.mask & LVIF_TEXT)
			{
				_sntprintf_s(szString, _countof(szString), _TRUNCATE,
					TEXT("Item %d"), lpdi->item.iItem + 1);
				_tcsncpy_s(lpdi->item.pszText, lpdi->item.cchTextMax,
					szString, _TRUNCATE);
			}

			if (lpdi->item.mask & LVIF_IMAGE)
			{
				lpdi->item.iImage = 0;
			}
		}
	}
	return 0;

	case LVN_ODCACHEHINT:
	{
		LPNMLVCACHEHINT   lpCacheHint = (LPNMLVCACHEHINT)lParam;
		/*
		This sample doesn't use this notification, but this is sent when the
		ListView is about to ask for a range of items. On this notification,
		you should load the specified items into your local cache. It is still
		possible to get an LVN_GETDISPINFO for an item that has not been cached,
		therefore, your application must take into account the chance of this
		occurring.
		*/
	}
	return 0;

	case LVN_ODFINDITEM:
	{
		LPNMLVFINDITEM lpFindItem = (LPNMLVFINDITEM)lParam;
		/*
		This sample doesn't use this notification, but this is sent when the
		ListView needs a particular item. Return -1 if the item is not found.
		*/
	}
	return 0;
	}

	return 0;
}

void SwitchView(HWND hwndListView, DWORD dwView)
{
	DWORD dwStyle = GetWindowLong(hwndListView, GWL_STYLE);

	SetWindowLong(hwndListView, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | dwView);
	ResizeListView(hwndListView, GetParent(hwndListView));
}

BOOL DoContextMenu(HWND hWnd,
	WPARAM wParam,
	LPARAM lParam)
{
	HWND  hwndListView = (HWND)wParam;
	HMENU hMenuLoad,
		hMenu;

	if (hwndListView != GetDlgItem(hWnd, ID_LISTVIEW))
		return FALSE;

	hMenuLoad = LoadMenu(hInst, MAKEINTRESOURCE(IDM_CONTEXT_MENU_CHILD));
	hMenu = GetSubMenu(hMenuLoad, 0);

	UpdateMenu(hwndListView, hMenu);

	TrackPopupMenu(hMenu,
		TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		LOWORD(lParam),
		HIWORD(lParam),
		0,
		hWnd,
		NULL);

	DestroyMenu(hMenuLoad);

	return TRUE;
}

BOOL InitInstance(HINSTANCE hInstance,
	int nCmdShow)
{
	HWND     hWnd;
	TCHAR    szTitle[MAX_PATH] = TEXT("");

	hInst = hInstance;

	LoadString(hInst, IDS_APPTITLE_CHILD, szTitle, sizeof(szTitle) / sizeof(szTitle[0]));

	/* Create a main window for this application instance.  */
	hWnd = CreateWindowEx(0,
		g_ClassName,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	/* If window could not be created, return "failure" */

	if (!hWnd)
		return FALSE;

	/* Make the window visible; update its client area; and return "success" */

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;

}

HWND CreateListView(HINSTANCE hInstance, HWND hwndParent)
{
	DWORD       dwStyle;
	HWND        hwndListView;
	HIMAGELIST  himlSmall;
	HIMAGELIST  himlLarge;
	BOOL        bSuccess = TRUE;

	dwStyle = WS_TABSTOP |
		WS_CHILD |
		WS_BORDER |
		WS_VISIBLE |
		LVS_AUTOARRANGE |
		LVS_REPORT |
		LVS_OWNERDATA;

	hwndListView = CreateWindowEx(WS_EX_CLIENTEDGE,          // ex style
		WC_LISTVIEW,               // class name - defined in commctrl.h
		TEXT(""),                        // dummy text
		dwStyle,                   // style
		0,                         // x position
		0,                         // y position
		0,                         // width
		0,                         // height
		hwndParent,                // parent
		(HMENU)ID_LISTVIEW,        // ID
		hInst,                   // instance
		NULL);                     // no extra data

	if (!hwndListView)
		return NULL;

	ResizeListView(hwndListView, hwndParent);

	//set the image lists
	himlSmall = ImageList_Create(16, 16, ILC_COLORDDB | ILC_MASK, 1, 0);
	himlLarge = ImageList_Create(32, 32, ILC_COLORDDB | ILC_MASK, 1, 0);

	if (himlSmall && himlLarge)
	{
		HANDLE hIcon;

		//set up the small image list
		hIcon = LoadImage(hInst, MAKEINTRESOURCE(IDI_DISK_CHILD),
			IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
		ImageList_AddIcon(himlSmall, (HICON)hIcon);

		//set up the large image list
		hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DISK_CHILD));
		ImageList_AddIcon(himlLarge, (HICON)hIcon);

		ListView_SetImageList(hwndListView, himlSmall, LVSIL_SMALL);
		ListView_SetImageList(hwndListView, himlLarge, LVSIL_NORMAL);
	}

	return hwndListView;
}

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

LRESULT CALLBACK MainWndProc(HWND hWnd,
	UINT uMessage,
	WPARAM wParam,
	LPARAM lParam)
{
	static HWND hwndListView;

	switch (uMessage)
	{
	case WM_CREATE:
		// create the TreeView control
		hwndListView = CreateListView(hInst, hWnd);

		//initialize the TreeView control
		InitListView(hwndListView);

		break;

	case WM_NOTIFY:
		return ListViewNotify(hWnd, lParam);

	case WM_SIZE:
		ResizeListView(hwndListView, hWnd);
		break;

	case WM_INITMENUPOPUP:
		UpdateMenu(hwndListView, GetMenu(hWnd));
		break;

	case WM_CONTEXTMENU:
		if (DoContextMenu(hWnd, wParam, lParam))
			return FALSE;
		break;

	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDM_LARGE_ICONS_CHILD:
			SwitchView(hwndListView, LVS_ICON);
			break;

		case IDM_SMALL_ICONS_CHILD:
			SwitchView(hwndListView, LVS_SMALLICON);
			break;

		case IDM_LIST_CHILD:
			SwitchView(hwndListView, LVS_LIST);
			break;

		case IDM_REPORT:
			SwitchView(hwndListView, LVS_REPORT);
			break;

		case IDM_EXIT_CHILD:
			DestroyWindow(hWnd);
			break;

		case IDM_ABOUT_CHILD:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT_CHILD), hWnd, AboutDlgProc);
			break;

		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}
	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

BOOL InitApplication(HINSTANCE hInstance)
{
	WNDCLASSEX  wcex;
	ATOM        aReturn;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = (WNDPROC)MainWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wcex.lpszClassName = g_ClassName;
	wcex.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MAINICON_CHILD));
	wcex.hIconSm = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_MAINICON_CHILD), IMAGE_ICON, 16, 16, 0);

	aReturn = RegisterClassEx(&wcex);

	if (0 == aReturn)
	{
		WNDCLASS wc;

		wc.style = 0;
		wc.lpfnWndProc = (WNDPROC)MainWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MAINICON_CHILD));
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
		wc.lpszClassName = g_ClassName;

		aReturn = RegisterClass(&wc);
	}

	return aReturn;
}

LRESULT CALLBACK WndProc(HWND    hWnd,
	UINT    Msg,
	WPARAM  wParam,
	LPARAM  lParam)
{
	switch (Msg)
	{
	case WM_CREATE:
	{
		CLIENTCREATESTRUCT ccs;
		ccs.hWindowMenu = GetSubMenu(GetMenu(hWnd), 0);
		ccs.idFirstChild = ID_MDI_FIRSTCHILD;

		hWndClient = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			"MDICLIENT",
			NULL,
			WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			hWnd,
			(HMENU)ID_MDI_CLIENT,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			(LPVOID)&ccs);

		if (!hWndClient)
		{
			MessageBox(hWnd, "Failed To Create The Client Window", "Error", MB_OK);
		}
		ShowWindow(hWndClient, SW_SHOW);
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDM_FILE_EXIT:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM_FILE_NEW:
		{
			HWND hChild;
			CREATESTRUCT cs;
			InitCommonControls();
			ZeroMemory(&cs, sizeof(CREATESTRUCT));
			hChild = CreateWindowEx(
				WS_EX_MDICHILD,
				ChildClassName,
				"Child Window",
				WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				hWndClient,
				NULL,
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				&cs);
			if (!InitApplication(hInst));
			if (!hChild)
				MessageBox(hWnd, "Failed To Create The Child Window", "Error", MB_OK);
		}
		break;

		default:
		{
			if (LOWORD(wParam) >= ID_MDI_FIRSTCHILD)
				DefFrameProc(hWnd, hWndClient, Msg, wParam, lParam);
			else
			{
				HWND hChild;
				hChild = (HWND)SendMessage(hWndClient, WM_MDIGETACTIVE, 0, 0);
				if (hChild)
					SendMessage(hChild, WM_COMMAND, wParam, lParam);
			}
		}
		}

		return 0;
	}
	break;
	case WM_CLOSE:

		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefFrameProc(hWnd, hWndClient, Msg, wParam, lParam);
	}
	return 0;
}

INT WINAPI WinMain(HINSTANCE  hInstance,
	HINSTANCE  hPrevInstance,
	LPSTR      lpCmdLine,
	INT nCmdShow)
{
	InitCommonControls();

	WNDCLASSEX    wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = ClassName;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	wc.lpfnWndProc = (WNDPROC)MainWndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_3DSHADOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = ChildClassName;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Failed To Register The Child Window Class", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	HWND    hWnd;
	hInst = hInstance;
	hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		ClassName,
		"MDI",
		WS_OVERLAPPEDWINDOW|WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		600,
		400,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
	{
		MessageBox(NULL, "Window Creation Failed.", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	ShowWindow(hWnd, SW_SHOW);

	UpdateWindow(hWnd);

	MSG    Msg;

	while (GetMessage(&Msg, NULL, 0, 0))
	{
		if (!TranslateMDISysAccel(hWndClient, &Msg))
		{
			TranslateMessage(&Msg);

			DispatchMessage(&Msg);
		}
	}
	return Msg.wParam;

}

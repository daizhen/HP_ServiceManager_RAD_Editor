// EditorStart.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "EditorStart.h"
#include "HookRADDll.h"
#include "ProcessHelper.h"
#include <Shellapi.h>

#define MAX_LOADSTRING 100
#define WM_SHOWTASK (WM_USER + 0x100)

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void SetCurrentPath();
LPWSTR GetDLLPath();

NOTIFYICONDATA   nid; 	
HMENU notifyMenu;


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
					 int       nCmdShow)
{
	//Set the current path in reg table.
	SetCurrentPath();

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_EDITORSTART, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EDITORSTART));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_TRAY));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_EDITORSTART);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
   nid.hWnd=hWnd; 
   nid.uID=IDR_MAINFRAME; 
   nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP   ; 
   nid.uCallbackMessage=WM_SHOWTASK;
   nid.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON_TRAY)); 
   wsprintf(nid.szTip, TEXT("RAD Editor"));

   Shell_NotifyIcon(NIM_ADD,&nid);

   if (!hWnd)
   {
      return FALSE;
   }
   notifyMenu = LoadMenu(hInstance,MAKEINTRESOURCE(IDC_EDITORSTART));
   notifyMenu = GetSubMenu(notifyMenu, 0);

   ShowWindow(hWnd,SW_HIDE);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//


void EndHook(HWND hWnd)
{
	Unhook();
}
void StartHook(HWND hWnd)
{
	ProcessHelper helper;
	vector<PROCESSENTRY32> processes = helper.GetProcessList();

	DWORD processId = 0;
	DWORD parentProcessId = 0;

	DWORD threadId = 0;

	for(int i=0;i<processes.size();i++)
	{
		PROCESSENTRY32 currentProcess = processes.at(i);
		TCHAR* processName = currentProcess.szExeFile;
		if(_tcscmp(processName,TEXT("ServiceManager.exe")) ==0)
		{
			parentProcessId = currentProcess.th32ProcessID;

			BOOL hasChildProcess = FALSE;
			//break;
			for(int j=0;j<processes.size();j++)
			{
				PROCESSENTRY32 currentProcess = processes.at(j);
				TCHAR* processName = currentProcess.szExeFile;
				if(currentProcess.th32ParentProcessID == parentProcessId)
				{
					hasChildProcess = TRUE;
					processId = currentProcess.th32ProcessID;
					vector<THREADENTRY32> threads = helper.GetThreadList(processId);
					threadId = helper.GetWindowThreadId(processId);
					break;
				}
			}
			if (!hasChildProcess)
			{
				PROCESSENTRY32 currentProcess = processes.at(i);
				processId = currentProcess.th32ProcessID;
				vector<THREADENTRY32> threads = helper.GetThreadList(processId);
				threadId = helper.GetWindowThreadId(processId);
			}

			if(processId!=0 && threadId!=0)
			{
				break;
			}
		}
	}

	if(processId!=0 && threadId!=0)
	{
		EndHook(hWnd);
		//HWND smHwnd = GetTopWindow( FindWindow(TEXT("SWT_Window0"),NULL));
		//if(smHwnd)
		//{
			//DWORD threadId = GetWindowThreadProcessId(smHwnd, &processId);
			SetHook(threadId);		
			SetProcessEventHook(processId);
		//}
		//else
		//{
		//	MessageBox(hWnd,TEXT("SM client not started"),TEXT("ERROR"),MB_OK);
		//}
	}
	else
	{
		MessageBox(hWnd,TEXT("SM client not started"),TEXT("ERROR"),MB_OK);
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	POINT currentPoint;
	switch (message)
	{
	case WM_SHOWTASK:

		switch(lParam)
		{
		case WM_RBUTTONDOWN:
		case WM_LBUTTONDOWN:
			GetCursorPos(&currentPoint);
			SetForegroundWindow(hWnd);
			TrackPopupMenu(notifyMenu, TPM_RIGHTBUTTON, currentPoint.x, currentPoint.y, 0, hWnd, NULL); 
			break;
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			EndHook(hWnd);
			DestroyWindow(hWnd);
			break;
		case ID_FILE_START:
			StartHook(hWnd);
			break;
		case ID_FILE_END:
			EndHook(hWnd);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		EndHook(hWnd);
		PostQuitMessage(0);
		break;
	case WM_CLOSE:

		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	TCHAR fileAndPath[200];
	TCHAR pathName[200];

	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void SetCurrentPath()
{
	const TCHAR g_szRegSubKey[] = TEXT("Software\\Benjamin\\Current Path");
	
	TCHAR filePath[400];
	LONG fileNameLength = GetCurrentDirectory(300,filePath);
	filePath[fileNameLength]=TEXT('\0');

	LONG result = RegDeleteKey(HKEY_CURRENT_USER, g_szRegSubKey);

   // Create the registry key to hold the info
   HKEY hkey;
   result = RegCreateKeyEx(HKEY_CURRENT_USER, g_szRegSubKey, 0, NULL, 
      REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hkey, NULL);

   result = RegSetValueEx(hkey, TEXT("Editor Path"), 0, REG_SZ, (PBYTE)filePath, 400);

   RegCloseKey(hkey);
}


LPWSTR GetDLLPath()
{
	TCHAR dllFullPath[MAX_PATH];

	TCHAR path[MAX_PATH];
	DWORD dataLength = sizeof(path);

	TCHAR keyName[20];
	DWORD keyLength = _countof(keyName);

	const TCHAR g_szRegSubKey[] = TEXT("Software\\Benjamin\\Current Path");
	HKEY hkey;
	DWORD dwType;
	LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, g_szRegSubKey,
		0, KEY_QUERY_VALUE, &hkey);

	result = RegEnumValue(hkey, 0, keyName,&keyLength, 
		NULL, &dwType, (PBYTE) path, &dataLength);
	keyName[keyLength] = TEXT('\0');


	wsprintf(dllFullPath,TEXT("%s\\SciLexer.DLL"),path);


	RegCloseKey(hkey);

	return dllFullPath;
}
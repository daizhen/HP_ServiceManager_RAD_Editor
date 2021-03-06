// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#define HookDll  extern "C" __declspec(dllexport)

#include "HookRADDll.h"
#include <commctrl.h>
#include "SuperRADEditor.h"
#include "ScintillaCtrl.h"
#include "RadLexer.h"
#include "RadParser.h"
#include "TransformCharandWchar.h"
#include "Scintilla.h"
#include "DialogFindReplace.h"
#include "UniConversion.h"

#pragma comment(lib,"ComCtl32.Lib") 

#define  NUM_BUTTONS 5

#pragma data_seg("ShareData")
HINSTANCE instance = NULL;
HHOOK message_hHook = NULL;
HHOOK proc_hHook = NULL;
HWINEVENTHOOK event_hHook = NULL;
TCHAR dllPathAndName[MAX_PATH];

#pragma data_seg()


TCHAR* currentValue;
BOOL messageNotHandled = FALSE;

// Forward references
BOOL CheckMenuItemExists(HMENU contextMenu);
int GetCustomMenuItemPosition(HMENU contextMenu);
void ProcessErrorMessage(TCHAR* ErrorText,HWND hwnd);
LRESULT WINAPI GetMsgProcNew(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI GetMsgProcCallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
VOID WINAPI WinEventsProc( HWINEVENTHOOK hWinEventHook, DWORD dwEvent, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime );

INT_PTR CALLBACK MainForm(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

INT_PTR CALLBACK HandleNotification(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void OnFormatRAD(HWND hWnd);
void OnSerializeRAD(HWND hWnd);
void  GetDLLPath(LPWSTR dllFullPath);
LPWSTR GetBuffer(int length);
HWND CreateAToolBar(HWND hWndParent);
void SetToolTip(LPTOOLTIPTEXT tooTip);
DWORD  g_dwThreadIdDIPS = 0;
int lowMessage;
int highMessage;
HMODULE sciLexerModule ;
HWND dialogReplace;
HWND toolBar;
HWND focusedControl;


ScintillaCtrl scintillaCtrl;


BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		instance = hModule;
		break;
	case DLL_THREAD_ATTACH:
		//Test("DLL_THREAD_ATTACH\r\n");
		break;
	case DLL_THREAD_DETACH:
		//Test("DLL_THREAD_DETACH\r\n");
		break;
	case DLL_PROCESS_DETACH:
		Unhook();
		break;
	}
	return TRUE;
}


HookDll void SetHook(DWORD threadId)
{
	g_dwThreadIdDIPS = GetCurrentThreadId();
	message_hHook = SetWindowsHookEx(WH_CALLWNDPROC , GetMsgProcCallWndProc, instance, threadId);
	proc_hHook  = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProcNew, instance, threadId);
}

HookDll void SetProcessEventHook(DWORD processId)
{
	event_hHook =  ::SetWinEventHook( EVENT_MIN , EVENT_MAX , instance, WinEventsProc,processId, 0, WINEVENT_INCONTEXT  );
}

HookDll void Unhook()
{
	if(message_hHook)
	{
		UnhookWindowsHookEx(message_hHook);
		message_hHook= NULL;
	}		
	if(proc_hHook)
	{
		UnhookWindowsHookEx(proc_hHook);
		proc_hHook = NULL;
	}

	if(event_hHook)
	{
		UnhookWinEvent(event_hHook);
		event_hHook = NULL;
	}
	if (sciLexerModule!=NULL)
	{
		FreeLibrary(sciLexerModule);
	}	
}


void ProcessErrorMessage(TCHAR* ErrorText,HWND hwnd)
{
	TCHAR ErrorMsg[400];
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER| 
		FORMAT_MESSAGE_FROM_SYSTEM, 
		NULL,GetLastError(), 
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 
		0,
		NULL);
	wsprintf(ErrorMsg, TEXT("\"%s\" has the Error : %s"),ErrorText,lpMsgBuf);
	MessageBox(hwnd,ErrorMsg, TEXT("Error"), MB_ICONSTOP);
	LocalFree(lpMsgBuf);
}




/*
This method used only to handle the ctrl+E to trigger the editor, by send the COMMAND 0x3100 
to the thread, then handled the Command message in next method.
*/
LRESULT WINAPI GetMsgProcNew(int nCode, WPARAM wParam, LPARAM lParam) 
{
	static BOOL bFirstTime = TRUE;
	string str;
	char chars[100];

	MSG *temMsg = (MSG *)lParam;

	CWPSTRUCT* cwpStruct;

	WINDOWINFO info;
	TCHAR className[200];

	int wmId ;
	int wmEvent;

	LONG length ;
	TCHAR content[2000];
	CWPSTRUCT temStruct;

	switch(temMsg->message)
	{
	case WM_KEYUP:

		if (GetKeyState(VK_CONTROL) & 0x80 && temMsg->wParam == 'R')
		{
			length = SendMessage(WindowFromPoint(temMsg->pt),WM_GETTEXT,10000,(LPARAM)content);
			GetClassName(temMsg->hwnd,className,50);
			if(wcscmp(className,TEXT("Scintilla"))!=0)
			{
				temStruct.hwnd = temMsg->hwnd;
				temStruct.message =0x0200;
				temStruct.lParam =(LPARAM)(temMsg->hwnd);
				SendMessage(temMsg->hwnd,WM_COMMAND,0x3100,(LPARAM)(temMsg->hwnd));
			}
		}
		break;

		//case WM_INITMENUPOPUP:
		//	//AppendMenu((HMENU)(temMsg->wParam),MF_STRING,1,TEXT("TEST0"));
		//	//AppendMenu((HMENU)(temMsg->wParam),MF_STRING,2,TEXT("TEST7"));
		//	//AppendMenu((HMENU)(temMsg->wParam),MF_STRING,3,TEXT("TEST8"));

		//	//
		//	//sprintf(chars,"HMENU Message : %d\t wParam:%x\t lParam%u\r\n",temMsg->wParam,temMsg->wParam,temMsg->lParam);
		//	//str = string(chars);

		//	//LogFile(str);
		//	//LogFile("\r\n");

		//	break;
		//case WM_CONTEXTMENU:

		//	//sprintf(chars,"WM_CONTEX Message : %d\t wParam:%c\t lParam%u\r\n",temMsg->wParam,temMsg->wParam,temMsg->lParam);
		//	//str = string(chars);

		//	//LogFile(str);
		//	//LogFile("\r\n");
		//	break;

	case WM_RBUTTONDOWN:
		//
		focusedControl = (HWND)(temMsg->hwnd);
		break;

	case WM_COMMAND:

		cwpStruct = (CWPSTRUCT*)(temMsg->lParam);
		if(cwpStruct != NULL)
		{
			//wmId    = LOWORD(cwpStruct->wParam);
			//wmEvent = HIWORD(cwpStruct->wParam);
			//sprintf(chars,"COMMAND Message From MSG HOOK : %x\t wmId:%x\t wmEvent%x\r\n",cwpStruct->message,wmId,wmEvent);
			//str = string(chars);
		}
		else 
		{
			lowMessage = LOWORD(temMsg->wParam);
			highMessage = HIWORD(temMsg->wParam);

			//sprintf(chars,"COMMAND Message XXXXXX : 0x%x\t wParam:%d\t lParam: 0x%x\r\n",temMsg->message,temMsg->wParam,temMsg->lParam);
			//str = string(chars);
			//LogFile(str);
			//LogFile("\r\n");

			if(9990 == lowMessage)
			{
				//length = SendMessage(WindowFromPoint(temMsg->pt),WM_GETTEXT,10000,(LPARAM)content);
				//GetClassName(temMsg->hwnd,className,50);
				//if(wcscmp(className,TEXT("Scintilla"))!=0)
				//{
				//	temStruct.hwnd = temMsg->hwnd;
				//	temStruct.message =0x0200;
				//	temStruct.lParam =(LPARAM)(temMsg->hwnd);
				//	SendMessage(focusedControl,WM_COMMAND,0x0100,(LPARAM)(focusedControl));
				//}
				SendMessage(focusedControl,WM_COMMAND,0x3100,(LPARAM)(focusedControl));
			}
			////GetWindowInfo(currentHwnd,&info);
			//GetClassName(temMsg->hwnd,className,50);

			//MessageBox(temMsg->hwnd,className,TEXT("Class"),MB_OK);

		}
		break;
		//case WM_SHOWWINDOW:
		//	sprintf(chars,"WM_SHOWWINDOW Message : %x\t wParam:%u\t lParam%u\r\n",temMsg->message,temMsg->wParam,temMsg->lParam);
		//	str = string(chars);

		//	LogFile(str);
		//	LogFile("\r\n");

	default :
		break;
	}
	return(CallNextHookEx(message_hHook, nCode, wParam, lParam));
}



LRESULT WINAPI GetMsgProcCallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	static BOOL bFirstTime = TRUE;
	string str;
	char chars[100];
	CWPSTRUCT* cwpStruct;

	cwpStruct = (CWPSTRUCT*)lParam;

	if(WM_COMMAND == cwpStruct->message)
	{
		int wmId    = LOWORD(cwpStruct->wParam);
		int wmEvent = HIWORD(cwpStruct->wParam);

		TCHAR content[40000];
		TCHAR className[100];

		if(wmEvent == 0)
		{
			if(wmId == 0x3100)
			{
				HWND temWnd = (HWND)(cwpStruct->lParam);
				HWND parentWnd = GetParent(temWnd);

				TCHAR className[400];
				int classNameLength = GetClassName(temWnd,className,50);
				className[classNameLength] =  TEXT('\0');

				//int contentLength = wcslen(currentValue);
				//for(int i=0;i<contentLength;i++)
				//{
				//	currentValue[i] = TEXT('\0');
				//}

				if(wcscmp(className,TEXT("SysListView32")) == 0)
				{
					wstring allListString;

					int nMaxItems = ListView_GetItemCount(temWnd);
					for(int i=0;i<nMaxItems;i++)
					{
						TCHAR szName[18000];
						ListView_GetItemText(temWnd, i, 0, szName,_countof(szName));
						if(wcslen(szName)>1)
						{
							allListString.append(szName);
							allListString.append(TEXT(";"));
						}
						allListString.append(TEXT("\r\n"));
					}

					currentValue = new TCHAR[allListString.length()+1];
					for(int i =0;i<allListString.length();i++)
					{
						currentValue[i] = allListString.c_str()[i];
					}
					currentValue[allListString.length()] = TEXT('\0');
				}
				else
				{
					LONG length = GetWindowTextLength(temWnd);
					currentValue = new TCHAR[length+1];

					length= SendMessage(temWnd,WM_GETTEXT,length+1,(LPARAM)currentValue);
					currentValue[length] = TEXT('\0');
				}
				DialogBox(instance, MAKEINTRESOURCE(IDD_DIALOGMAIN), parentWnd, MainForm);

				RECT windowRect;
				GetWindowRect(temWnd,&windowRect);

				int winX = windowRect.left;
				int winY = windowRect.top;


				POINT winPoint ;
				winPoint.x = winX+5;
				winPoint.y = winY+3;

				temWnd = WindowFromPoint(winPoint);

				SetFocus(temWnd);
				//MessageBox(temWnd,content,TEXT("Info"),MB_OK);

				//SetFocus(temWnd);
				wsprintf(content,TEXT("TEST value"));

				//Should be listview.
				HWND parentWindow = WindowFromPoint(winPoint);

				winX = winX+5;
				winY = winY+3;

				SendMessage(parentWindow,WM_MOVE,NULL,0);
				SendMessage(parentWindow,WM_LBUTTONDOWN,MK_LBUTTON,(winX + winY<<16));
				SendMessage(parentWindow,WM_LBUTTONUP,MK_LBUTTON,(winX + winY<<16));
			}

		}
	}
	else if(WM_MENUCOMMAND == cwpStruct->message)
	{
		MessageBox(cwpStruct->hwnd,TEXT("TEST"),TEXT("Info"),MB_OK);
	}
	else if( WM_RBUTTONDOWN  == cwpStruct->message)
	{
		MessageBox(cwpStruct->hwnd,TEXT("WM_RBUTTONDOWN"),TEXT("Info"),MB_OK);
	}
	else if(WM_INITMENUPOPUP == cwpStruct->message)
	{
		HMENU temHwnd= (HMENU)cwpStruct->wParam;
		if(!CheckMenuItemExists(temHwnd))
		{
			AppendMenu(temHwnd,MF_STRING,9990,TEXT("Edit in SuperEditor"));
		}
	}
	else if(WM_UNINITMENUPOPUP == cwpStruct->message)
	{
		HMENU temHwnd= (HMENU)cwpStruct->wParam;
		//if(!CheckMenuItemExists(temHwnd))
		//{
		//	AppendMenu(temHwnd,MF_STRING,9990,TEXT("Edit in SuperEditor"));
		//}
		int position = GetCustomMenuItemPosition(temHwnd);
		if(position>=0)
		{
			DeleteMenu(temHwnd,position,MF_BYPOSITION);
		}
	}
	else if( WM_SETFOCUS ==cwpStruct->message)
	{
		focusedControl = cwpStruct->hwnd;
	}
	else
	{
		//Nothing to do.
	}
	if (bFirstTime) 
	{
		// The DLL just got injected.
		bFirstTime = FALSE;
	}
	return(CallNextHookEx(proc_hHook, nCode, wParam, lParam));
}

VOID WINAPI WinEventsProc( HWINEVENTHOOK hWinEventHook, DWORD dwEvent, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime )
{

	string str;
	char ch ;
	char chars[1000];
	TCHAR text[1000];
	LONG length ;
	char className[200];
	int compareResult = -1;
	switch( dwEvent )
	{
	case EVENT_OBJECT_FOCUS:

		GetClassNameA(hwnd,className,50);
		compareResult= strcmp(className,"Edit");

		if( compareResult == 0 )
		{
			if(messageNotHandled)
			{
				SendMessage(hwnd,WM_SETTEXT,40000,(LPARAM)currentValue);
				messageNotHandled = FALSE;
				if(currentValue)
				{
					delete[] currentValue;
				}
			}
		}
		break;
	}
}
// Message handler for Main dialog.
INT_PTR CALLBACK MainForm(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	int wmId, wmEvent;
	RECT rect;
	int width;
	int height ;
	char* temCharText;
	TCHAR* contentStr;
	int contentLength = 0;
	TransformCharandWchar transform;

	switch (message)
	{
	case WM_NOTIFY:
		if(((LPNMHDR)lParam)->code == TTN_NEEDTEXT) 
		{ 
			LPTOOLTIPTEXT       lpToolTipText   =   (LPTOOLTIPTEXT)lParam; 
			lpToolTipText-> hinst   =   instance; 
			//lpToolTipText-> lpszText   = MAKEINTRESOURCE(lpToolTipText-> hdr.idFrom); 
			SetToolTip(lpToolTipText);
		} 
		else
		{
			HandleNotification( hDlg,  message,  wParam,  lParam);
		}
		break;
	case WM_SIZE:
		//GetWindowRect(hWnd,&rect);
		GetClientRect(hDlg,&rect);
		width = (int)(rect.right -  rect.left);
		height = (int)(rect.bottom - rect.top);
		SendMessage(toolBar,WM_SIZE,wParam,lParam);
		MoveWindow(scintillaCtrl.scinlliaWnd,0,31,width,height - 31,TRUE);

		break;
	case WM_INITDIALOG:

		if (sciLexerModule==NULL)
		{

			TCHAR filePath[MAX_PATH];
			GetDLLPath(filePath);

			sciLexerModule = LoadLibrary(filePath);
		}
		if (sciLexerModule==NULL)
		{	
			MessageBox(NULL,
				_T ("The Scintilla DLL could not be loaded."),
				_T ("Error loading Scintilla"),
				MB_OK | MB_ICONERROR);
		}
		toolBar = CreateAToolBar(hDlg);
		scintillaCtrl.Create(hDlg);
		scintillaCtrl.SetEnrionment();

		scintillaCtrl.SetText(wstring(currentValue));
		if(currentValue)
		{
			delete[] currentValue;
		}

		dialogReplace = CreateDialog(instance, MAKEINTRESOURCE(IDD_SEARCH), hDlg, FindAndReplaceProc);
		ShowWindow(dialogReplace,SW_HIDE);

		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, About);
			break;
		case ID_FILE_EXIT:
		case IDCANCEL:
			//if(dialogReplace!=NULL)
			//{
			//	DestroyWindow(dialogReplace);
			//	dialogReplace=NULL;
			//}
			EndDialog(hDlg, LOWORD(wParam));
			break;

		case ID_EDIT_FORMATRAD:
			//Format RAD
			OnFormatRAD(hDlg);
			break;
		case ID_EDIT_SERIALIZERAD:
			//Serialize RAD

			OnSerializeRAD(hDlg);
			break;
		case ID_EDIT_FINDREPLACE:
			//Find and replace
			ShowWindow(dialogReplace,SW_SHOW);
			break;
		case ID_EDIT_COMMENTLINES:
			scintillaCtrl.CommentSelectedLines();
			break;
		case ID_EDIT_UNOMMENTLINES:
			scintillaCtrl.UnCommentSelectedLines();
			break;
		case ID_FILE_SAVE:

			messageNotHandled = TRUE;

			temCharText  = scintillaCtrl.GetText();
			contentStr = ConvertToUTF16FromUTF8(temCharText);
			if(temCharText)
			{
				delete temCharText;
			}
			contentLength = wcslen(contentStr);
			currentValue = new TCHAR[contentLength+1];

			for(int i=0;i< contentLength;i++)
			{
				currentValue[i] = contentStr[i];
			}
			currentValue[contentLength]=TEXT('\0');
			delete[] contentStr;

			EndDialog(hDlg, LOWORD(wParam));
			break;
		case WM_INITMENU:
			ShowWindow(dialogReplace,SW_SHOW);
			break;

		default:
			return DefWindowProc(hDlg, message, wParam, lParam);
		}
		break;
	}
	return (INT_PTR)FALSE;
}



HookDll void Test(string str)
{
	LogFile(str);
	return;
}

HookDll void LogFile(string  message)
{
	//HANDLE hTempFile = CreateFile(TEXT("C:\\Users\\daizhen\\Documents\\Visual Studio 2008\\Projects\\HookDll\\HookEntity\\Message.txt"),  // file name 
	//	GENERIC_READ | GENERIC_WRITE, // open for read/write 
	//	0,                            // do not share 
	//	NULL,                         // default security 
	//	OPEN_ALWAYS ,                // overwrite existing file
	//	FILE_APPEND_DATA ,        // normal file 
	//	NULL);                        // no template 

	//SetFilePointer(hTempFile, 0, NULL, FILE_END);
	//DWORD realSize;
	//WriteFile(hTempFile, (void *)message.c_str(),message.size(),&realSize,NULL);

	//CloseHandle(hTempFile);
}


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
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




void OnSerializeRAD(HWND hWnd)
{

	char* chars = scintillaCtrl.GetText();
	Statements* statements =NULL;

	TransformCharandWchar transform;
	try
	{
		string str(chars);
		RadLexer lexer(str);
		lexer.Build();

		RadParser parser(lexer.GetTokens());
		scintillaCtrl.ScrollToLeftSide();
		statements = parser.Parse();
		scintillaCtrl.SetText(statements->ToString());
		delete statements;
	}
	catch(RadSyntaxException e)
	{
		MessageBoxEx(hWnd,transform.CharToWchar(e.what()),
			_T ("Syntax Error"),
			MB_OK | MB_ICONERROR,NULL);

		if(statements!=NULL)
		{
			delete statements;
		}
	}
	catch(exception& ex)
	{
		MessageBoxEx(hWnd,transform.CharToWchar(ex.what()),
			_T ("Syntax Error"),
			MB_OK | MB_ICONERROR,NULL);
	}
}



void OnFormatRAD(HWND hWnd)
{
	char* chars = scintillaCtrl.GetText();
	Statements* statements =NULL;
	TransformCharandWchar transform;
	try
	{
		string str(chars);
		RadLexer lexer(str);
		lexer.Build();

		RadParser parser(lexer.GetTokens());
		scintillaCtrl.ScrollToLeftSide();
		statements = parser.Parse();
		scintillaCtrl.SetText(statements->ToString(0));
		delete statements;
	}
	catch(RadSyntaxException& e)
	{
		MessageBoxEx(hWnd,transform.CharToWchar(e.what()),
			_T ("Syntax Error"),
			MB_OK | MB_ICONERROR,NULL);

		if(statements!=NULL)
		{
			delete statements;
		}
	}
	catch(exception& ex)
	{
		MessageBoxEx(hWnd,transform.CharToWchar(ex.what()),
			_T ("Syntax Error"),
			MB_OK | MB_ICONERROR,NULL);

		if(statements!=NULL)
		{
			delete statements;
		}
	}
}




void  GetDLLPath(LPWSTR dllFullPath)
{
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
}

LPWSTR GetBuffer(int length)
{
	return (LPWSTR)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,length*sizeof(TCHAR));
}

BOOL FreeBuffer()
{
	return HeapFree(GetProcessHeap(),HEAP_NO_SERIALIZE,currentValue);
}

INT_PTR CALLBACK HandleNotification(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int LastProcessedChar = 0;

	LPNMHDR lpnmhdr = (LPNMHDR)lParam;
	if(lpnmhdr->hwndFrom==scintillaCtrl.scinlliaWnd)
	{
		SCNotification* notify = (SCNotification*)(lParam);

		if(notify)
		{

			if(notify->nmhdr.code == SCN_CHARADDED)
			{
				LastProcessedChar = notify->ch;
			}
			if(notify->nmhdr.code == SCN_UPDATEUI && LastProcessedChar!=0)
			{
				int pos = scintillaCtrl.GetCurrentPosition(); //取得当前位置
				int line = scintillaCtrl.GetLineFromPosition(pos); //取得当前行
				//如果最后输入的字符是右括号的话就自动让当前行缩进和它匹配的左括号所在行一致
				if( LastProcessedChar == ')' &&
					isspace(scintillaCtrl.GetCharFromPosition(pos-2)) && //要求右括号左边是空白字符
					LastProcessedChar!=0)
				{
					//找前一个单词起始位置，这里用它来确定右括号左边是否全是空白字符
					int startpos = scintillaCtrl.GetWordStartPosition(pos-1);
					int linepos = scintillaCtrl.GetLinePosition(line); //当前行起始位置
					if(startpos == linepos) //这样相当于判断右括号左边是否全是空白字符
					{
						int othpos = scintillaCtrl.GetBracketMatchPosition(pos-1); //得到对应的左括号所在的位置
						int othline = scintillaCtrl.GetLineFromPosition(othpos);  //左括号所在行
						int nIndent = scintillaCtrl.GetLineIndent(othline);//左括号所在行的缩进值
						// 替换右括号前面的空白字符，使之与左括号缩进一致

						string str(nIndent,' ');
						scintillaCtrl.SetTargetStart(startpos);
						scintillaCtrl.SetTargetEnd( pos-1);
						scintillaCtrl.ReplaceTarget(str);
					} 
				}
				// 如果输入的是回车，则保持与上一行缩进一致
				// 如果上一行最后有效字符为左括号，就多缩进四个空格
				if(LastProcessedChar == '\n')
				{
					if(line > 0)
					{
						// 得到上一行缩进设置
						int nIndent = scintillaCtrl.GetLineIndent(line-1);
						// 查找上一行最后一个有效字符（非空白字符）
						int nPrevLinePos = scintillaCtrl.GetLinePosition(line-1);
						int previousChar = ' ';
						int nextChar=' ';
						for(int p = pos-2; p>=nPrevLinePos && previousChar>0 && isspace(previousChar); p--)
						{
							previousChar=scintillaCtrl.GetCharFromPosition(p);
						}

						int temPosition = pos;
						while(true)
						{
							if(!nextChar || !isspace(nextChar) || nextChar=='\n')
							{
								break;
							}
							nextChar= scintillaCtrl.GetCharFromPosition(temPosition);
							temPosition++;
						}

						// 如果是左括号，就多缩进四格
						if(previousChar && previousChar=='(') 
						{
							//To match the bracket, keep the indent.
							if(nextChar && nextChar==')')
							{
								//Nothing to do.
							}
							else
							{
								nIndent+=4;
							}
						}
						string str(nIndent,' ');
						scintillaCtrl.ReplaceSelected(str);
					}
				}
				LastProcessedChar = 0;
			}
		}
	}

	return (INT_PTR)FALSE;
}




HWND CreateAToolBar(HWND hWndParent)
{
	HWND hWndToolbar;
	TBADDBITMAP tb;
	int index, stdidx;
	INITCOMMONCONTROLSEX icex;//assert to load comctl32.dll

	// Toolbar buttons
	TBBUTTON tbButtons [] =
	{
		{0, ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_FLAT, 0L, 0},
		{1, ID_EDIT_FORMATRAD, TBSTATE_ENABLED, TBSTYLE_FLAT, 0L, 0},
		{2, ID_EDIT_SERIALIZERAD, TBSTATE_ENABLED, TBSTYLE_FLAT, 0L, 0},
		//{0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0L, 0},//separator
		{3, ID_EDIT_FINDREPLACE, TBSTATE_ENABLED, TBSTYLE_FLAT,0L, 0},
		{4, ID_EDIT_COMMENTLINES, TBSTATE_ENABLED, TBSTYLE_FLAT,0L, 0},
		{5, ID_EDIT_UNOMMENTLINES, TBSTATE_ENABLED, TBSTYLE_FLAT,0L, 0}

	};

	// Ensure that the common control DLL is loaded.
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES; 
	InitCommonControlsEx(&icex);

	// Create the toolbar
	hWndToolbar = CreateToolbarEx (hWndParent,
		WS_CHILD | WS_BORDER | WS_VISIBLE | TBSTYLE_TOOLTIPS,
		32980, 11, (HINSTANCE)HINST_COMMCTRL, IDB_STD_SMALL_COLOR,
		(LPCTBBUTTON)&tbButtons, 0, 0, 0, 100, 50, sizeof (TBBUTTON));


	HIMAGELIST hImageList = ImageList_Create(16,16,ILC_COLOR32,0,4); 

	ImageList_Add(hImageList,LoadBitmap(instance,MAKEINTRESOURCE(IDB_BITMAPSAVE)),NULL);
	ImageList_Add(hImageList,LoadBitmap(instance,MAKEINTRESOURCE(IDB_BITMAPFORMAT)),NULL);
	ImageList_Add(hImageList,LoadBitmap(instance,MAKEINTRESOURCE(IDB_BITMAPSERIALIZE)),NULL);
	ImageList_Add(hImageList,LoadBitmap(instance,MAKEINTRESOURCE(IDB_BITMAPFIND)),NULL);

	ImageList_Add(hImageList,LoadBitmap(instance,MAKEINTRESOURCE(IDB_BITMAPCOMMENT)),NULL);
	ImageList_Add(hImageList,LoadBitmap(instance,MAKEINTRESOURCE(IDB_BITMAPUNCOMMENT)),NULL);


	SendMessage(hWndToolbar, TB_SETIMAGELIST, 0, (LPARAM)hImageList);

	tb.hInst = HINST_COMMCTRL;
	tb.nID = IDB_VIEW_SMALL_COLOR;
	stdidx = SendMessage (hWndToolbar, TB_ADDBITMAP, 12, (LPARAM)&tb);

	int iIndexString[NUM_BUTTONS];
	for (index = 0; index < NUM_BUTTONS; index++)
	{
		char str[] =  "TEST";
		iIndexString[index]=SendMessage(hWndToolbar, TB_ADDSTRING, (WPARAM)instance, (LPARAM)(LPSTR)str);
		tbButtons[index].iString=iIndexString[index];
	}
	SendMessage (hWndToolbar, TB_ADDBUTTONS, 6, (LONG) &tbButtons);
	return hWndToolbar;
}

void SetToolTip(LPTOOLTIPTEXT lpToolTipText)
{
	unsigned int toolbarItemId = lpToolTipText-> hdr.idFrom;
	if(toolbarItemId == ID_EDIT_FORMATRAD)
	{
		lpToolTipText-> lpszText   = TEXT("Format RAD"); 
	}
	else if(toolbarItemId == ID_EDIT_FINDREPLACE)
	{
		lpToolTipText-> lpszText   = TEXT("Find & Replace"); 
	}
	else if(toolbarItemId == ID_EDIT_SERIALIZERAD)
	{
		lpToolTipText-> lpszText   = TEXT("Serialize the RAD to one line"); 
	}
	else if(toolbarItemId == ID_FILE_EXIT)
	{
		lpToolTipText-> lpszText   = TEXT("EXIT"); 
	}	
	else if(toolbarItemId == ID_FILE_SAVE)
	{
		lpToolTipText-> lpszText   = TEXT("Save"); 
	}
	else if(toolbarItemId == ID_EDIT_COMMENTLINES)
	{
		lpToolTipText-> lpszText   = TEXT("Comment selected lines"); 
	}
	else if(toolbarItemId == ID_EDIT_UNOMMENTLINES)
	{
		lpToolTipText-> lpszText   = TEXT("Uncomment selected lines"); 
	}
	else
	{
		//Nothing to do.
	}
}

BOOL CheckMenuItemExists(HMENU contextMenu)
{
	int itemCount = GetMenuItemCount(contextMenu);
	for(int i=0;i<itemCount;i++)
	{
		//GetMenuItemID(contextMenu,i)
		TCHAR itemString[200];
		GetMenuString(contextMenu,i,itemString,200,MF_BYPOSITION);
		if(lstrcmp(itemString,TEXT("Edit in SuperEditor")) == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}
int GetCustomMenuItemPosition(HMENU contextMenu)
{
	int itemCount = GetMenuItemCount(contextMenu);
	for(int i=0;i<itemCount;i++)
	{
		//GetMenuItemID(contextMenu,i)
		TCHAR itemString[200];
		GetMenuString(contextMenu,i,itemString,200,MF_BYPOSITION);
		if(lstrcmp(itemString,TEXT("Edit in SuperEditor")) == 0)
		{
			return i;
		}
	}
	return -1;
}
#include "StdAfx.h"
#include "Resource.h"
#include "DialogFindReplace.h"
#include "string"
#include "UniConversion.h"


using namespace std;

INT_PTR CALLBACK FindAndReplaceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	TCHAR textToSearch[2000] ;
	TCHAR textForReplace[2000] ;
	string stringToSearch;
	string stringForReplace;
	int searchResult =-1;
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_CLOSE:
		ShowWindow(dialogReplace,SW_HIDE);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			//DestroyWindow(hDlg);
			ShowWindow(dialogReplace,SW_HIDE);
			//dialogReplace = NULL;
			return (INT_PTR)TRUE;
		}
		else if(LOWORD(wParam) == IDC_BUTTONFINDNEXT)
		{
			GetDlgItemText(hDlg,IDC_EDITSTRINGTOFIND,textToSearch,2000);
			char* temString = ConvertToUTF8FromUTF16(textToSearch);
			stringToSearch = string(temString);

			searchResult = scintillaCtrl.SearchText(stringToSearch,scintillaCtrl.GetCurrentPosition());
			if(searchResult >= 0)
			{
				scintillaCtrl.SetSelected(searchResult,searchResult + stringToSearch.length());
				scintillaCtrl.SetCurrentPosition(searchResult + stringToSearch.length());
			}
			delete[] temString;
		}
		else if(LOWORD(wParam) == IDC_BUTTONREPLACE)
		{
			GetDlgItemText(hDlg,IDC_EDITREPLACE,textForReplace,2000);
			char* targetString = ConvertToUTF8FromUTF16(textForReplace);

			stringForReplace = string(targetString);
			scintillaCtrl.ReplaceSelected(stringForReplace);
			
			delete[] targetString;
		}
		else if(LOWORD(wParam) == IDC_BUTTONFINDALL)
		{
			GetDlgItemText(hDlg,IDC_EDITSTRINGTOFIND,textToSearch,2000);
			char* temString = ConvertToUTF8FromUTF16(textToSearch);
			stringToSearch = string(temString);
			scintillaCtrl.MarkAll(stringToSearch);
			delete[] temString;
		}
		else if(LOWORD(wParam) == IDC_BUTTONREPLACEALL)
		{
			GetDlgItemText(hDlg,IDC_EDITSTRINGTOFIND,textToSearch,2000);
			char* sourceString = ConvertToUTF8FromUTF16(textToSearch);
			stringToSearch = string(sourceString);

			GetDlgItemText(hDlg,IDC_EDITREPLACE,textForReplace,2000);
			char* targetString = ConvertToUTF8FromUTF16(textForReplace);
			stringForReplace = string(targetString);

			scintillaCtrl.ReplaceAll(stringToSearch,stringForReplace);

			delete[] sourceString;
			delete[] targetString;
		}
		else 
		{
			//Nothing to do
		}
		break;
	}

	return (INT_PTR)FALSE;
}

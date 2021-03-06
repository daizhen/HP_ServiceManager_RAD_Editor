#include "StdAfx.h"
#include "ScintillaCtrl.h"

#include "Scintilla.h"
#include "SciLexer.h"
#include <winUser.h>
#include "UniConversion.h"

ScintillaCtrl::ScintillaCtrl(void)
{
}

ScintillaCtrl::~ScintillaCtrl(void)
{
}

LRESULT ScintillaCtrl::SendEditor(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return SendMessage(scinlliaWnd,Msg, wParam,lParam);
}


int ScintillaCtrl::GetPrefixBlanksCount(int line)
{
	int totalLength = GetLength();
	int blankCount = 0;
	int currentPosition = GetLinePosition(line);
	while(currentPosition <totalLength && GetCharFromPosition(currentPosition)==' ')
	{
		blankCount++;
		currentPosition++;
	}
	return blankCount;
}
bool ScintillaCtrl::IsBlankLine(int line)
{
	int totalLength = GetLength();
	int currentPosition = GetLinePosition(line);
	while(currentPosition <totalLength && GetCharFromPosition(currentPosition)==' ')
	{
		currentPosition++;
	}

	if(currentPosition  == totalLength)
	{
		return true;
	}
	else if(GetCharFromPosition(currentPosition) == '\r' || GetCharFromPosition(currentPosition) =='\n')
	{
		return true;
	}
	else
	{
		return false;
	}
}

int ScintillaCtrl::Create(HWND parentWnd)
{
	RECT parentRect;

	GetWindowRect(parentWnd,&parentRect);

	int width = parentRect.right  - parentRect.left;
	int height =  parentRect.bottom  - parentRect.top;
	scinlliaWnd = CreateWindowEx(0,
		_T("Scintilla"),_T("Scintilla"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
		0,31,750,580 - 31,parentWnd,NULL, NULL,NULL);

	SendEditor(SCI_GETDIRECTFUNCTION, 0, 0);

	SendEditor(SCI_GETDIRECTPOINTER, 0, 0);

	return 1;
} 

char* ScintillaCtrl::GetText()
{
	unsigned int len = SendEditor(SCI_GETTEXT, 0, 0);
	char *chText = new char[len];
	SendEditor(SCI_GETTEXT, len, (LPARAM)chText);
	return chText;
}

void ScintillaCtrl::SetText(string value)
{
	SendEditor(SCI_SETTEXT, 0, (LPARAM)value.c_str());
}

void ScintillaCtrl::SetText(wstring value)
{
	unsigned int contentLength = UTF8Length(value.c_str(),value.length()*4);
	char* temCharText = new char[contentLength+1];
	UTF8FromUTF16(value.c_str(),value.length(),temCharText,contentLength);
	SendEditor(SCI_SETTEXT, 0, (LPARAM)temCharText);
	delete[] temCharText;
}

void ScintillaCtrl::SetEnrionment()
{
	const char* keywords=
		"if then else for to do while in and or not true false isin";

	//Sets the font Size of the whole content.
	SendEditor(SCI_STYLESETSIZE,STYLE_DEFAULT,12);
	SendEditor(SCI_STYLESETFONT,STYLE_DEFAULT,(LPARAM) "Courier New");
	SendEditor(SCI_STYLECLEARALL);

	SendEditor(SCI_SETLEXER, SCLEX_CPP);
	SendEditor(SCI_SETKEYWORDS,0,(LPARAM)keywords);
	SendEditor(SCI_STYLESETFORE, SCE_C_WORD, 0x00FF0000);   //Key
	SendEditor(SCI_STYLESETFORE, SCE_C_STRING, 0x001515A3); //string
	SendEditor(SCI_STYLESETFORE, SCE_C_CHARACTER, 0x00008000); //char
	SendEditor(SCI_STYLESETFORE, SCE_C_COMMENT, 0x00008000);//Comments
	SendEditor(SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x00008000);//Comments

	SendEditor(SCI_STYLESETFORE,STYLE_BRACELIGHT,0xFF000000);

	SendEditor(SCI_STYLESETCHARACTERSET,STYLE_DEFAULT, SC_CHARSET_GB2312);

	SendEditor(SCI_SETTABWIDTH, 5);

	SendEditor(SCI_SETCARETLINEVISIBLE, TRUE);
	SendEditor(SCI_SETCARETLINEBACK, 0xb0ffff);

	SendEditor(SCI_SETMARGINWIDTHN ,0, 50);
	SendEditor(SCI_STYLESETFORE,STYLE_LINENUMBER,0x00AF912A);


	//SendMessage(SCI_CLEARALLCMDKEYS,0,0);
	//SendMessage(SCI_ASSIGNCMDKEY,'V' + ((SCMOD_CTRL) << 16), SCI_PASTE);

	SendEditor(SCI_CLEARCMDKEY,((int)'K')|((SCMOD_CTRL)<<16));
	SendEditor(SCI_CLEARCMDKEY,((int)'F')|((SCMOD_CTRL)<<16));
	SendEditor(SCI_CLEARCMDKEY,((int)'E')|((SCMOD_CTRL)<<16));
	SendEditor(SCI_SETCODEPAGE,SC_CP_UTF8);
	SendEditor(SCI_SETKEYSUNICODE,TRUE);


	SetMultiSelection(true);
	SetAdditionalSelectionTyping(true);	
	//SendMessage(SCI_CLEARCMDKEY,((int)'E')|((SCMOD_CTRL)<<16));
	//SendMessage(SCI_ASSIGNCMDKEY,((int)'V')|((SCMOD_CTRL)<<16), SCI_PASTE);

	//SendMessage(SCI_ASSIGNCMDKEY,((int)'v')|((SCMOD_CTRL)<<16), SCI_PASTE);
	//SendMessage(SCI_ASSIGNCMDKEY,'C' |((SCMOD_CTRL | SCMOD_SHIFT)<<16), SCI_COPY);
	//SendMessage(SCI_ASSIGNCMDKEY,'X' | ((SCMOD_CTRL | SCMOD_SHIFT)<<16), SCI_CUT);
	//SendEditor(SCI_ASSIGNCMDKEY,'Y'|((SCMOD_CTRL | SCMOD_SHIFT)<<16), SCI_REDO);
	//SendEditor(SCI_ASSIGNCMDKEY,'Z'|(SCMOD_ALT <<16), SCI_UNDO);

	//SendMessage(STYLE_BRACELIGHT);
	//SendMessage(STYLE_BRACEBAD);
	//SendMessage(STYLE_INDENTGUIDE);
}

void ScintillaCtrl::ReDo()
{
	LRESULT result =  SendEditor(SCI_CANREDO);
	if(result)
	{
		SendEditor(SCI_REDO);
	}
}

void ScintillaCtrl::UnDo()
{
	LRESULT result =  SendEditor(SCI_CANUNDO);
	if(result)
	{
		SendEditor(SCI_UNDO);
	}
}

void ScintillaCtrl::Cut()
{
	SendEditor(SCI_CUT);
}
void ScintillaCtrl::Copy()
{

	SendEditor(SCI_COPY);
}
void ScintillaCtrl::Paste()
{
	if(CanPaste())
	{
		SendEditor(SCI_PASTE);
	}
}

bool ScintillaCtrl::CanPaste()
{

	LRESULT result = SendEditor(SCI_CANPASTE);
	if(result)
	{
		return true;
	}

	return false;
}

void ScintillaCtrl::InsertString(int line,int positionOfLine,string str)
{
	int linePosition = GetLinePosition(line);
	InsertString(linePosition+positionOfLine,str);
}
void ScintillaCtrl::InsertString(int position,string str)
{
	this->Replace(str,position,position);
}


void ScintillaCtrl::ScrollToLeftSide()
{
	//ScrollWindow(scinlliaWnd,0,0,NULL,NULL);

	RECT rect;
	::GetClientRect(scinlliaWnd,&rect);
	::SetScrollPos(scinlliaWnd,SB_HORZ,0,TRUE);
	int pos = ::GetScrollPos(scinlliaWnd,SB_HORZ);
	::InvalidateRect(scinlliaWnd,&rect,TRUE);

	//::ScrollWindow(scinlliaWnd,0,0,&rect,&rect);	
}


int ScintillaCtrl::GetCurrentPosition()
{
	return SendEditor(SCI_GETCURRENTPOS);
}
int ScintillaCtrl::GetLinePosition(int line)
{
	return SendEditor(SCI_POSITIONFROMLINE,line);
}
char ScintillaCtrl::GetCharFromPosition(int position)
{
	return SendEditor(SCI_GETCHARAT,position);
}
int ScintillaCtrl::GetWordStartPosition(int position)
{
	return SendEditor(SCI_WORDSTARTPOSITION,position,NULL);
}
int ScintillaCtrl::GetLineStartPosition(int line)
{
	return SendEditor(SCI_POSITIONFROMLINE,line);
}
int ScintillaCtrl::GetBracketMatchPosition(int position)
{
	return SendEditor(SCI_BRACEMATCH,position);
}
int ScintillaCtrl::GetLineFromPosition(int position)
{
	return SendEditor(SCI_LINEFROMPOSITION,position);
}
int ScintillaCtrl::GetLineIndent(int line)
{
	return SendEditor(SCI_GETLINEINDENTATION,line);
}
void ScintillaCtrl::SetTargetStart(int position)
{
	SendEditor(SCI_SETTARGETSTART,position);
}
void ScintillaCtrl::SetTargetEnd(int position)
{
	SendEditor(SCI_SETTARGETEND,position);
}
void ScintillaCtrl::ReplaceTarget(string newStr)
{
	SendEditor(SCI_REPLACETARGET,newStr.size(),(LPARAM)newStr.c_str());
}
void ScintillaCtrl::ReplaceSelected(string newStr)
{
	SendEditor(SCI_REPLACESEL,0,(LPARAM)newStr.c_str());
}

int ScintillaCtrl::SearchNext(string str)
{
	return SendEditor(SCI_SEARCHNEXT,NULL,(LPARAM)str.c_str());
}
int ScintillaCtrl::SearchPrevious(string str)
{
	return SendEditor(SCI_SEARCHPREV,NULL,(LPARAM)str.c_str());
}


int ScintillaCtrl::SearchText(string textToSearch,int start)
{
	Sci_TextToFind textStruct;
	textStruct.chrg.cpMin=start;
	textStruct.chrg.cpMax= GetLength();

	textStruct.lpstrText = (char*)(textToSearch.c_str());

	int result = SendEditor(SCI_FINDTEXT,NULL,(LPARAM)&textStruct);
	if(result<0)
	{
		return -1;
	}
	return textStruct.chrgText.cpMin;
}

vector<int> ScintillaCtrl::FindAll(string textToSearch)
{
	vector<int> positions;

	SendEditor(SCI_SEARCHANCHOR, NULL, NULL);

	int textLength = textToSearch.length();
	int position = -1;
	position = SearchText(textToSearch,0);
	while( position >=0)
	{
		positions.push_back(position);
		//int searchStartPosition = position + textToSearch.length();
		int newPosition= this->SearchText(textToSearch,position+textLength);
		if(newPosition == position)
		{
			break;
		}
		else
		{
			position =newPosition;
		}
	}
	return positions;
}

void ScintillaCtrl::Replace(string textToReplace,int rangeStart,int rangeEnd)
{
	SetTargetStart(rangeStart);
	SetTargetEnd(rangeEnd);
	ReplaceTarget(textToReplace);
}

void ScintillaCtrl::SetSelected(int start, int end)
{
	SendEditor(SCI_SETSEL,start,(LPARAM)end);
}

int ScintillaCtrl::GetLength()
{
	return SendEditor(SCI_GETLENGTH, NULL, NULL);
}

void ScintillaCtrl::SetCurrentPosition(int position)
{
	SendEditor(SCI_SETCURRENTPOS, position, NULL);
}

void ScintillaCtrl::SetMultiSelection(bool multipleSelection)
{
	SendEditor(SCI_SETMULTIPLESELECTION, multipleSelection == false?0:1, NULL);
}
bool ScintillaCtrl::GetMultiSelection()
{
	int result = SendEditor(SCI_GETMULTIPLESELECTION, NULL, NULL);
	return !(result==0);
}
void ScintillaCtrl::SetAdditionalSelectionTyping(bool additionalSelectionTyping)
{
	SendEditor(SCI_SETADDITIONALSELECTIONTYPING, additionalSelectionTyping==false?0:1, NULL);
}
bool ScintillaCtrl::GetAdditionalSelectionTyping()
{
	int result = SendEditor(SCI_GETADDITIONALSELECTIONTYPING, NULL, NULL);
	return !(result==0);
}

void ScintillaCtrl::SetMultiPaste(int multiPaste)
{
	SendEditor(SCI_SETMULTIPASTE, multiPaste, NULL);
}
int ScintillaCtrl::GetMultiPaste()
{
	return SendEditor(SCI_GETMULTIPASTE, NULL, NULL);
}
int ScintillaCtrl::GetSelections()
{
	return SendEditor(SCI_GETSELECTIONS, NULL, NULL);
}
void ScintillaCtrl::ClearSelections()
{
	SendEditor(SCI_CLEARSELECTIONS, NULL, NULL);
}
void ScintillaCtrl::SetSelection(int start, int end)
{
	SendEditor(SCI_SETSELECTION, start, end);
}
void ScintillaCtrl::AddSelection(int start, int end)
{
	SendEditor(SCI_ADDSELECTION,start, end);
}
void ScintillaCtrl::SetMainSelection(int selection)
{
	SendEditor(SCI_SETMAINSELECTION,selection, NULL);
}

int ScintillaCtrl::GetMainSelection()
{
	return SendEditor(SCI_GETMAINSELECTION,NULL, NULL);
}

void ScintillaCtrl::SetSelectionStart(int selection, int pos)
{
	SendEditor(SCI_SETSELECTIONNSTART,selection, pos);
}
int ScintillaCtrl::GetSelectionStart(int selection)
{
	return 	SendEditor(SCI_GETSELECTIONNSTART,selection, NULL);
}

void ScintillaCtrl::SetSelectionEnd(int selection, int pos)
{
	SendEditor(SCI_SETSELECTIONNEND,selection, pos);
}
int ScintillaCtrl::GetSelectionEnd(int selection)
{
	return 	SendEditor(SCI_GETSELECTIONNEND,selection, NULL);
}


/*
Utility methods
*/

void ScintillaCtrl::MarkAll(vector<int> starts, string str)
{
	int stringLength = str.length();
	if(starts.size() > 0)
	{
		SetSelection(starts.at(0),starts.at(0) + stringLength);
	}

	for(int i=0;i<starts.size();i++)
	{
		AddSelection(starts.at(i),starts.at(i) + stringLength);
	}
}
void ScintillaCtrl::MarkAll(string str)
{
	MarkAll(FindAll(str),str);
}

void ScintillaCtrl::ReplaceAll(vector<int> starts, string oldStr,string newString)
{
	for(int i=starts.size() - 1;i>=0;i--)
	{
		int start = starts.at(i);
		Replace(newString,start,start+oldStr.length());
	}
}

void ScintillaCtrl::ReplaceAll(string oldStr,string newString)
{
	vector<int> starts = FindAll(oldStr);
	ReplaceAll(starts,oldStr,newString);
}

void ScintillaCtrl::CommentSelectedLines()
{
	int selectionStart = GetSelectionStart(0);
	int selecteionEnd = GetSelectionEnd(0);

	int startLine = GetLineFromPosition(selectionStart);
	int endLine =  GetLineFromPosition(selecteionEnd);

	for(int i=startLine;i<=endLine;i++)
	{
		if(!IsBlankLine(i))
		{
			int prefixBlankCount = GetPrefixBlanksCount(i);
			InsertString(i,prefixBlankCount,"//");
		}
	}
}

void ScintillaCtrl::UnCommentSelectedLines()
{
	int totalLength = GetLength();
	int selectionStart = GetSelectionStart(0);
	int selecteionEnd = GetSelectionEnd(0);

	int startLine = GetLineFromPosition(selectionStart);
	int endLine =  GetLineFromPosition(selecteionEnd);

	for(int i=startLine;i<=endLine;i++)
	{
		if(!IsBlankLine(i))
		{
			int currentLinePosition = GetLinePosition(i);
			int prefixBlankCount = GetPrefixBlanksCount(i);
			if(currentLinePosition + prefixBlankCount+2<totalLength &&
				GetCharFromPosition(currentLinePosition + prefixBlankCount) =='/' &&
				GetCharFromPosition(currentLinePosition + prefixBlankCount + 1) =='/')
			{
				int startPosition = currentLinePosition + prefixBlankCount;
				int endPosition = currentLinePosition + prefixBlankCount + 2;
				Replace("",startPosition,endPosition);
			}
		}
	}
}
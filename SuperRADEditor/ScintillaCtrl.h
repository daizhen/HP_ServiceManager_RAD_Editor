#pragma once

#include "string"
#include "vector"

using namespace std;

class ScintillaCtrl
{
public:
	HWND scinlliaWnd;
private:
	LRESULT SendEditor(UINT Msg, WPARAM wParam=0, LPARAM lParam=0);
	int GetPrefixBlanksCount(int line);
	bool IsBlankLine(int line);

public:

	BOOL Create(HWND pParentWnd);
	int GetLength();
	char* GetText();
    void SetText(string value);
    void SetText(wstring value);
    void SetEnrionment();
    void ReDo();
    void UnDo();
    void Cut();
    void Copy();
    void Paste();
	void InsertString(int line,int positionOfLine,string str);
	void InsertString(int position,string str);
	void ScrollToLeftSide();
    bool CanPaste();
	int GetCurrentPosition();
	void SetCurrentPosition(int position);
	int GetLinePosition(int line);
	char GetCharFromPosition(int position);
	int GetWordStartPosition(int position);
	int GetLineStartPosition(int line);
	int GetBracketMatchPosition(int position);
	int GetLineFromPosition(int position);
	int GetLineIndent(int line);
	void SetTargetStart(int position);
	void SetTargetEnd(int position);
	void ReplaceTarget(string newStr);
	void ReplaceSelected(string newStr);
	void SetSelected(int start, int end);

	int SearchText(string textToSearch,int start);
	int SearchNext(string str);
	int SearchPrevious(string str);
	vector<int> FindAll(string textToSearch);
	void Replace(string textToReplace,int rangeStart,int rangeEnd);

	/*
		Multi selection
	*/
	void SetMultiSelection(bool multipleSelection);
	bool GetMultiSelection();
	void SetAdditionalSelectionTyping(bool additionalSelectionTyping);
	bool GetAdditionalSelectionTyping();

	void SetMultiPaste(int multiPaste);
	int GetMultiPaste();
	int GetSelections();
	void ClearSelections();

	//This method will add a new selection 
	//And set this one as main selection
	void SetSelection(int start, int end);

	//This method will add a new selection 
	//And set this one as additional.
	void AddSelection(int start, int end);
	void SetMainSelection(int selection);
	int GetMainSelection();

	void SetSelectionStart(int selection, int pos);
	int GetSelectionStart(int selection);

	void SetSelectionEnd(int selection, int pos);
	int GetSelectionEnd(int selection);

	/*
		Utility methods
	*/
	void MarkAll(vector<int> starts, string str);
	void MarkAll(string str);
	void ReplaceAll(vector<int> starts, string oldStr,string newString);
	void ReplaceAll(string oldStr,string newString);
	void CommentSelectedLines();
	void UnCommentSelectedLines();
public:
	ScintillaCtrl(void);
	~ScintillaCtrl(void);
};

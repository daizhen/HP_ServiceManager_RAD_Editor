#pragma once
#include "list"
#include "vector"
#include "string"
#include "RadToken.h"

using namespace std;

class RadLexer
{
public:
	void Build();
	list<RadToken> GetTokens();
public:
	RadLexer(string rawData);
	~RadLexer(void);
   string ToString();

private:
	list<RadToken> tokens;
	
	//Index of the current cotent;
	int currentIndex;
	string rawProgram;
	string bufferString;

	//Current line
	int currentLine;

	//The position in current line.
	int positionOfLine;

private:
	void ClearContext();
	char GetCurrentChar();
    vector<char> determinedKeyChars;
    vector<char> undeterminedKeyChars;
    vector<string> keyWords;
    vector<string> boolConsts;

    bool IsDeterminedKeyChar(char ch);
    bool IsUndeterminedFirstChar(char ch);
    bool IsBlankOrEnd();
    bool IsDigit(char ch);
    bool IsCharacter(char ch);
    bool IsKeyWord(string str);
    bool IsBoolConst(string str);
    void ConstructDeterminedKeys();
    void ConstructUndeterminedFirstChars();
    void ConstructKeyWords();
    void ConstructBoolConst();

	RadToken GetVariableToken();
	RadToken GetCommentsToken();

	RadToken GetStringConstToken();
	RadToken GetStringIdenfier();
	RadToken GetDateConstToken();
	RadToken GetSignedNumber();
	string GetIntValue();
	string GetBoolConst();
    RadToken GetUndeterminedToken();
    RadToken GetUndeterminedKeyToken();
    RadToken GetLastToken();



	void SkipBlanks();
    
    //Move to next, just increase the point index
    //If extend the end of the string return false
    //Else return true
    bool MoveNext();
};


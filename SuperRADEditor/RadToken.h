#pragma once
#include "string"

using namespace std;

class RadToken
{
private:

	string tokenValue;
	int line;
	int positionOfLine;
public:
	
	enum TokenType
	{
		Number,
		StringConst,
		DateConst,
		Key,
		Variable,
        StringIndentifier,
        BoolConst,
		Comments
	} tokenType;

    string ToString();
public:
	RadToken(void);
	RadToken(TokenType,string);
	string GetTokenValue();
	void SetTokenValue(string value);
	TokenType GetTokenType();
	void SetTokenType(TokenType tokenType);

	int GetLine();
	void SetLine(int lineNumber);
	int GetPositionOfLine();
	void SetPositionOfLine(int position);


	~RadToken(void);
};

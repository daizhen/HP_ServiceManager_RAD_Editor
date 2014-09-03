#include "StdAfx.h"
#include "RadToken.h"


RadToken::RadToken(void)
{
}

RadToken::RadToken(TokenType type ,string value)
{
	tokenType = type;
	tokenValue = value;
	positionOfLine=0;
	line=0;
}


RadToken::~RadToken(void)
{
}

RadToken::TokenType RadToken::GetTokenType()
{
	return tokenType;
}

void RadToken::SetTokenType(RadToken::TokenType type)
{
	tokenType = type;
}

string RadToken::GetTokenValue()
{
	return tokenValue;
}

void RadToken::SetTokenValue(string value)
{
	tokenValue = value;
}

string RadToken::ToString()
{
	string valueString;
	if(tokenType == Number)
	{
		valueString.append("Number");
	}
	else if(tokenType == StringConst)
	{
		valueString.append("StringConst");
	} 
	else if(tokenType == DateConst)
	{
		valueString.append("DateConst");
	} 
	else if(tokenType == Key)
	{
		valueString.append("Key");
	} 
	else if(tokenType == Variable)
	{
		valueString.append("Variable");
	}
	else
	{
		//Nothing to do.
	}
	valueString.append(":\t\t");
	valueString.append(tokenValue);
	return valueString;
}



int RadToken::GetLine()
{
	return line;
}
void RadToken::SetLine(int lineNumber)
{
	line = lineNumber;
}
int RadToken::GetPositionOfLine()
{
	return positionOfLine;
}
void RadToken::SetPositionOfLine(int position)
{
	positionOfLine = position;
}
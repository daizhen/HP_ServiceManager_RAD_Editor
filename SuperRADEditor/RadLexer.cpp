#include "StdAfx.h"
#include "RadLexer.h"

RadLexer::RadLexer(string rawData)
{
	currentLine = 1;
	positionOfLine = 1;

	rawProgram = rawData;
	currentIndex = 0;

	ConstructDeterminedKeys();
	ConstructUndeterminedFirstChars();
	ConstructKeyWords();
	ConstructBoolConst();
}

RadLexer::~RadLexer(void)
{
}

list<RadToken> RadLexer::GetTokens()
{
	return tokens;
}

void RadLexer::Build()
{
	int length = rawProgram.length();
	SkipBlanks();
	while(currentIndex < length)
	{
		char firstChar = rawProgram[currentIndex];
		if(firstChar == '$')
		{
			RadToken temToken = GetVariableToken();
			tokens.push_back(temToken);
		}
		else if(firstChar == '"')
		{
			RadToken temToken =GetStringConstToken();
			tokens.push_back(temToken);
		}
		else if(firstChar == '\'')
		{
			RadToken temToken = GetDateConstToken();
			tokens.push_back(temToken);
		}  
		//This is comments.
		else if(firstChar == '/' && currentIndex+1<length &&rawProgram[currentIndex+1] =='/' )
		{
			//No need to check the result of the method, because of the pre condition.
			MoveNext();
			tokens.push_back(GetCommentsToken());
			tokens.push_back(RadToken(RadToken::Key,";"));
		}
		else if(firstChar == '+' || firstChar == '-')
		{
			if(tokens.size() == 0 || GetLastToken().GetTokenValue() =="("
				||GetLastToken().GetTokenValue() =="if" || GetLastToken().GetTokenValue() =="then"
				||GetLastToken().GetTokenValue() =="else" ||GetLastToken().GetTokenValue() ==","
				||GetLastToken().GetTokenValue() ==";" || GetLastToken().GetTokenValue() =="while" 
				||GetLastToken().GetTokenValue() =="do"||GetLastToken().GetTokenValue() =="to"
				||GetLastToken().GetTokenValue() =="for"||GetLastToken().GetTokenValue() =="=")
			{
				//This sign is associated with a number.
				RadToken temToken = GetSignedNumber();
				tokens.push_back(temToken);
			}
			else
			{
				if(currentIndex+1<length && rawProgram[currentIndex+1] =='=' )
				{
					//+= , -= is a single sign.
					string str;
					str.push_back(firstChar);
					str.push_back(rawProgram[currentIndex+1]);
					RadToken temToken(RadToken::Key,str);
					//Set line info.
					temToken.SetLine(currentLine);
					temToken.SetPositionOfLine(positionOfLine);

					tokens.push_back(temToken);

					//Move two steps. One for next '=' and second for next char.
					MoveNext();
					MoveNext();

				}
				else
				{
					// +, - is a single char sign.
					string str;
					str.push_back(firstChar);
					RadToken temToken(RadToken::Key,str);
					//Set line info.
					temToken.SetLine(currentLine);
					temToken.SetPositionOfLine(positionOfLine);

					tokens.push_back(temToken);
					MoveNext();
				}
			}
		}
		else if(firstChar == '*' || firstChar == '/')
		{
			if(currentIndex+1<length && rawProgram[currentIndex+1] =='=' )
			{
				//*= , /= is a single sign.
				string str;
				str.push_back(firstChar);
				str.push_back(rawProgram[currentIndex+1]);
				RadToken temToken(RadToken::Key,str);
				//Set line info.
				temToken.SetLine(currentLine);
				temToken.SetPositionOfLine(positionOfLine);

				tokens.push_back(temToken);

				//Move two steps. One for next '=' and second for next char.
				MoveNext();
				MoveNext();

			}
			else
			{
				// *, / is a single char sign.
				string str;
				str.push_back(firstChar);
				RadToken temToken(RadToken::Key,str);
				//Set line info.
				temToken.SetLine(currentLine);
				temToken.SetPositionOfLine(positionOfLine);

				tokens.push_back(temToken);
				MoveNext();
			}
		}
		else if(firstChar == '{')
		{
			if(MoveNext())
			{
				if(GetCurrentChar() == '[')
				{
					MoveNext();
					RadToken temToken( RadToken::Key,"{[");
					temToken.SetLine(currentLine);
					temToken.SetPositionOfLine(positionOfLine);
					tokens.push_back(temToken);
				}
				else
				{
					RadToken temToken( RadToken::Key,"{");

					temToken.SetLine(currentLine);
					temToken.SetPositionOfLine(positionOfLine);
					tokens.push_back(temToken);
				}
			}
			else
			{
				RadToken temToken( RadToken::Key,"{");

				temToken.SetLine(currentLine);
				temToken.SetPositionOfLine(positionOfLine);

				tokens.push_back(temToken);
			}
		}
		else if(firstChar == ']')
		{
			if(MoveNext())
			{
				if(GetCurrentChar() != '}')
				{
					RadToken temToken( RadToken::Key,"]");

					temToken.SetLine(currentLine);
					temToken.SetPositionOfLine(positionOfLine);
					tokens.push_back(temToken);
				}
				else
				{
					MoveNext();
					RadToken temToken( RadToken::Key,"]}");

					temToken.SetLine(currentLine);
					temToken.SetPositionOfLine(positionOfLine);
					tokens.push_back(temToken);
				}
			}
			else
			{
				MoveNext();
				RadToken temToken( RadToken::Key,"]}");

				temToken.SetLine(currentLine);
				temToken.SetPositionOfLine(positionOfLine);
				tokens.push_back(temToken);
			}
		} 
		else if(IsDigit(firstChar) ||
			IsCharacter(firstChar) ||
			firstChar == '_')
		{
			RadToken temToken =  GetUndeterminedToken();
			tokens.push_back(temToken);
		}
		else if(IsDeterminedKeyChar(firstChar))
		{
			string str;
			str.push_back(firstChar);
			RadToken temToken(RadToken::Key,str);

			temToken.SetLine(currentLine);
			temToken.SetPositionOfLine(positionOfLine);
			tokens.push_back(temToken);
			MoveNext();
		}
		else if(IsUndeterminedFirstChar(firstChar))
		{
			RadToken temToken =  GetUndeterminedKeyToken();
			tokens.push_back(temToken);
		}
		else 
		{
			string errorString = "not valid char:";
			errorString.push_back(firstChar);
			throw exception(errorString.c_str());
		}

		SkipBlanks();
	}
}

void RadLexer::SkipBlanks()
{
	while(currentIndex < rawProgram.length())
	{
		if(rawProgram[currentIndex] == ' ' || 
			rawProgram[currentIndex] == '\t' ||
			rawProgram[currentIndex] == '\r' ||
			rawProgram[currentIndex] == '\n')
		{
			// Store the current line and position of the line.
			if(rawProgram[currentIndex]== '\n')
			{
				positionOfLine = 1;
				currentLine ++;
			}
			else
			{
				positionOfLine++;
			}

			currentIndex++;
		}
		else
		{
			break;
		}
	}
}

char RadLexer::GetCurrentChar()
{
	return rawProgram[currentIndex];
}

RadToken  RadLexer::GetUndeterminedToken()
{
	RadToken token;
	token.SetLine(currentLine);
	token.SetPositionOfLine(positionOfLine);

	string temStr;
	temStr.push_back(GetCurrentChar());

	bool isNumber = true;

	while(MoveNext())
	{
		char currentChar = GetCurrentChar();
		if(IsDigit(currentChar) || IsCharacter(currentChar)||
			currentChar=='.' || currentChar =='_')
		{
			temStr.push_back(currentChar);
			if(!IsDigit(currentChar))
			{
				isNumber = false;
			}
		}
		else
		{
			break;
		}
	}

	if(isNumber)
	{
		token.SetTokenType(RadToken::Number);
	}
	else if(IsKeyWord(temStr))
	{
		token.SetTokenType(RadToken::Key);
	}
	else if(IsBoolConst(temStr))
	{
		token.SetTokenType(RadToken::BoolConst);
	}
	else
	{
		token.SetTokenType(RadToken::StringIndentifier);
	}

	token.SetTokenValue(temStr);

	return token;
}


bool RadLexer::IsDeterminedKeyChar(char ch)
{
	for(int i=0;i<determinedKeyChars.size();i++)
	{
		if(ch == determinedKeyChars.at(i))
		{
			return true;
		}
	}
	return false;
}

bool RadLexer::IsUndeterminedFirstChar(char ch)
{
	for(int i=0;i<undeterminedKeyChars.size();i++)
	{
		if(ch == undeterminedKeyChars.at(i))
		{
			return true;
		}
	}
	return false;
}

bool RadLexer::IsBlankOrEnd()
{
	//Now end of the raw string.
	if(currentIndex >= rawProgram.length())
	{
		return true;
	}
	//Check if current is blank.
	if(rawProgram[currentIndex] == ' ' || 
		rawProgram[currentIndex] == '\t' ||
		rawProgram[currentIndex] == '\r' ||
		rawProgram[currentIndex] == '\n')
	{
		return true;
	}
	return false;
}
bool RadLexer::IsKeyWord(string str)
{
	for(int i=0;i<keyWords.size();i++)
	{
		if(str == keyWords.at(i))
		{
			return true;
		}
	}
	return false;
}


bool RadLexer::IsBoolConst(string str)
{
	for(int i=0;i<boolConsts.size();i++)
	{
		if(str == boolConsts.at(i))
		{
			return true;
		}
	}
	return false;
}

RadToken RadLexer::GetUndeterminedKeyToken()
{
	RadToken temToken;

	temToken.SetLine(currentLine);
	temToken.SetPositionOfLine(positionOfLine);

	temToken.SetTokenType(RadToken::Key);
	char firstChar = GetCurrentChar();
	string tokenString;
	tokenString.push_back(firstChar);

	MoveNext();
	if(!IsBlankOrEnd())
	{
		char nextChar = GetCurrentChar();
		if(nextChar == '#' || nextChar == '=')
		{
			tokenString.push_back(GetCurrentChar());
			MoveNext();
		}
	}
	temToken.SetTokenValue(tokenString);
	return temToken;
}

/*
Get variable token
*/
RadToken RadLexer::GetVariableToken()
{
	RadToken token;
	token.SetLine(currentLine);
	token.SetPositionOfLine(positionOfLine);

	string str("$");
	token.SetTokenType(RadToken::Variable);
	while(MoveNext())
	{
		char currentChar = GetCurrentChar();
		if(currentChar =='.' || IsDigit(currentChar) || IsCharacter(currentChar))
		{
			str.push_back(currentChar);
		}
		else
		{
			break;
		}
	}
	token.SetTokenValue(str);
	return token;
}

/*
Gets the string const token.
*/
RadToken RadLexer::GetStringConstToken()
{
	RadToken token;
	token.SetLine(currentLine);
	token.SetPositionOfLine(positionOfLine);

	string str("\"");
	token.SetTokenType(RadToken::StringConst);

	//Used to check whether the last char is escape char. that means \

	bool isLastEscapeChar= false;
	bool isSuccess = false;
	while(MoveNext())
	{
		char currentChar = GetCurrentChar();
		str.push_back(currentChar);
		if(currentChar == '"')
		{
			if(!isLastEscapeChar)
			{
				isSuccess = true;
				MoveNext();
				break;
			}
			else
			{
				isLastEscapeChar = false;
			}
		}
		else
		{
			if(currentChar == '\\' && !isLastEscapeChar)
			{
				isLastEscapeChar = true;
			}
			else
			{
				isLastEscapeChar = false;
			}
		}
	}

	if(!isSuccess)
	{
		throw exception("String const  not valid");
	}
	token.SetTokenValue(str);
	return token;
}

/*
Gets date const token.
*/
RadToken RadLexer::GetDateConstToken()
{
	RadToken token;
	token.SetLine(currentLine);
	token.SetPositionOfLine(positionOfLine);

	token.SetTokenType(RadToken::DateConst);
	string dateString("'");
	bool isSuccess = false;

	while(MoveNext())
	{
		char currentChar = GetCurrentChar();
		dateString.push_back(currentChar);
		if(currentChar == '\'')
		{
			MoveNext();
			isSuccess = true;
			break;
		}
	}
	if(!isSuccess)
	{
		throw exception("Date const not valid");
	}
	token.SetTokenValue(dateString);
	return token;
}

void RadLexer::ConstructDeterminedKeys()
{
	determinedKeyChars.push_back(',');
	determinedKeyChars.push_back(';');
	determinedKeyChars.push_back('(');
	determinedKeyChars.push_back(')');
	determinedKeyChars.push_back('=');
	determinedKeyChars.push_back('#');
	determinedKeyChars.push_back('}');
}
void RadLexer::ConstructUndeterminedFirstChars()
{
	//The following to entries are handled in other place.
	undeterminedKeyChars.push_back('{');
	undeterminedKeyChars.push_back(']');

	undeterminedKeyChars.push_back('~');
	undeterminedKeyChars.push_back('<');
	undeterminedKeyChars.push_back('>');
}

void  RadLexer::ConstructKeyWords()
{
	keyWords.push_back("do");
	keyWords.push_back("while");
	keyWords.push_back("for");
	keyWords.push_back("in");
	keyWords.push_back("do");
	keyWords.push_back("to");
	keyWords.push_back("if");
	keyWords.push_back("then");
	keyWords.push_back("else");
	keyWords.push_back("and");
	keyWords.push_back("or");
	keyWords.push_back("not");
	keyWords.push_back("isin");
}
bool RadLexer::MoveNext()
{
	currentIndex++;
	if(currentIndex>= rawProgram.length())
	{
		return false;
	}
	return true;
}

bool RadLexer::IsDigit(char ch)
{
	if(ch >='0' &&  ch <='9')
	{
		return true;
	}
	return false;
}

bool RadLexer::IsCharacter(char ch)
{

	if((ch >='A' &&  ch <='Z') ||
		(ch >='a' &&  ch <='z'))
	{
		return true;
	}
	return false;
}

string RadLexer::ToString()
{
	string valueString;
	for(list<RadToken>::iterator it = tokens.begin();it!=tokens.end();it++)
	{
		valueString.append((*it).ToString()).append("\r\n");
	}

	return valueString;
}

RadToken RadLexer::GetLastToken()
{
	list<RadToken>::iterator it = tokens.begin();
	RadToken temToken = *it;
	while(it!=tokens.end())
	{
		temToken = *it;
		it++;
	}
	return temToken;
}

void RadLexer::ConstructBoolConst()
{
	boolConsts.push_back("true");
	boolConsts.push_back("t");
	boolConsts.push_back("T");
	boolConsts.push_back("TRUE");
	boolConsts.push_back("Y");
	boolConsts.push_back("y");
	boolConsts.push_back("yes");
	boolConsts.push_back("YES");
	boolConsts.push_back("false");
	boolConsts.push_back("f");
	boolConsts.push_back("F");
	boolConsts.push_back("FALSE");
	boolConsts.push_back("N");
	boolConsts.push_back("n");
	boolConsts.push_back("NO");
	boolConsts.push_back("unknown");
}

RadToken RadLexer::GetSignedNumber()
{
	RadToken token;

	token.SetLine(currentLine);
	token.SetPositionOfLine(positionOfLine);
	string temStr;
	temStr.push_back(GetCurrentChar());

	bool isNumber = true;

	while(MoveNext())
	{
		char currentChar = GetCurrentChar();
		if(IsDigit(currentChar) || IsCharacter(currentChar)||
			currentChar=='.')
		{
			temStr.push_back(currentChar);
		}
		else
		{
			break;
		}
	}
	token.SetTokenType(RadToken::Number);
	token.SetTokenValue(temStr);

	return token;
}


RadToken RadLexer::GetCommentsToken()
{
	RadToken token;

	token.SetLine(currentLine);
	token.SetPositionOfLine(positionOfLine);

	string temStr;

	while(MoveNext())
	{
		char currentChar = GetCurrentChar();
		if(currentChar!='\r')
		{
			temStr.push_back(currentChar);
		}
		else
		{
			break;
		}
	}
	token.SetTokenType(RadToken::Comments);
	token.SetTokenValue(temStr);

	return token;
}
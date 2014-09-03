#include "StdAfx.h"
#include "RadParser.h"
#include "PrimitiveExpression.h"
#include "LogicalNotExpression.h"
#include "SpecialFunctionStatement.h"

RadParser::RadParser(list<RadToken> tokens)
{
	radTokens = tokens;
	ConstructSpecialFunctions();
	current = radTokens.begin();
}


RadParser::~RadParser(void)
{

}

list<RadToken> RadParser::GetRadTokens()
{
	return radTokens;
}

Statements* RadParser::Parse()
{
	Statements* statements = new Statements();
	while(!IsEnd())
	{
		BaseStatement* statement = NULL;
		if(MatchType(RadToken::Comments) || !Match(")"))
		{
			statement = GetStatement();
			statements->AddStatement(statement);
		}
		else
		{
			//if(!IsEnd())
			//{
			//    MoveNext();
			//}
			//break;
		}

		if(!IsEnd())
		{
			if(Match(")"))
			{
				if(!bracketFlags.empty())
				{
					break;
				}
				else
				{
					throw RadSyntaxException("')' is not expected",*current);
				}
			}
			else
			{
				if(!Match(";"))
				{
					if(IsEnd())
					{	
						throw exception("';' expected, in the end of file");
					}
					else
					{
						throw RadSyntaxException("';' expected",*current);
					}
				}
				MoveNext();
			}
		}
	}

	return statements;
}

bool RadParser::IsEnd()
{
	if(current == radTokens.end())
	{
		return true;
	}
	return false;
}

bool RadParser::MoveNext()
{
	current++;
	return !IsEnd();
}

bool RadParser::Match(string tokenValue)
{
	if(IsEnd())
	{
		return false;
	}
	if(current->GetTokenValue() == tokenValue)
	{
		return true;
	}
	return false;
}


bool RadParser::MatchType(RadToken::TokenType type)
{
	if(current->GetTokenType() == type)
	{
		return true;
	}
	return false;
}

bool RadParser::IsBracketStatements(/*list<RadToken>::iterator iterator,bool isTopLevel*/)
{
	int rightBracketWant = 1;
	list<RadToken>::iterator currentIterator = current;
	currentIterator++;
	bool isStatement = false;
	while(currentIterator!=radTokens.end())
	{
		if(currentIterator->GetTokenValue() == "(")
		{
			rightBracketWant++;
		}
		if(currentIterator->GetTokenValue() == ")")
		{
			rightBracketWant--;
		}
		if(rightBracketWant == 0)
		{
			break;
		}
		if(currentIterator->GetTokenValue() == ";" ||
			currentIterator->GetTokenValue() == "if" ||
			currentIterator->GetTokenValue() == "while")
		{
			isStatement = true;
			break;
		}
		currentIterator++;
	}

	if(!isStatement)
	{
		if(currentIterator==radTokens.end())
		{
			isStatement = false;
		}
		else if(currentIterator->GetTokenValue() == ")")
		{
			currentIterator++;
			if(currentIterator==radTokens.end())
			{
				isStatement = true;
			}
			else if(currentIterator->GetTokenValue() == "="
				||currentIterator->GetTokenValue() == "if"
				||currentIterator->GetTokenValue() == "else"
				||currentIterator->GetTokenValue() == "while"
				||currentIterator->GetTokenValue() == ")"
				||currentIterator->GetTokenValue() == ";")
			{
				isStatement = true;
			}
			else
			{
				isStatement = false;
			}
		}
	}
	//while((*currentIterator)!=radTokens.end() && (*currentIterator)->GetTokenValue() != ")")
	//{
	//    if((*currentIterator)->GetTokenValue() == "(")
	//    {
	//       if( IsBracketStatements(iterator,false))
	//       {
	//           return true;
	//       }
	//    }
	//    if((*currentIterator)->GetTokenValue() == ";")
	//    {
	//        return true;
	//    }
	//    currentIterator++;
	//}
	//if((*currentIterator) ==radTokens.end() )
	//{
	//    return false;
	//}
	//if((*currentIterator)->GetTokenValue() == ")")
	//{

	//}

	return isStatement;
}

/*
Gets the next term.
*/

BaseExpression* RadParser::GetTermExpression()
{
	BaseExpression* firstExpression = NULL;
	if(Match("("))
	{
		firstExpression = GetBracketExpression();
	}
	else if(MatchType(RadToken::StringIndentifier))
	{
		list<RadToken>::iterator temIterator = current;
		temIterator++;
		if(temIterator!=radTokens.end() && temIterator->GetTokenValue() == "(")
		{
			firstExpression = GetFunctionCallExpression();
		}
		else
		{
			firstExpression = new PrimitiveExpression();
			((PrimitiveExpression*)firstExpression)->SetValue(*current);
			MoveNext();
		}
	}
	else if(Match("{"))
	{
		firstExpression = GetArray();
	}
	else if(Match("{["))
	{
		firstExpression = GetStructure();
	}
	else if(MatchType(RadToken::Variable) ||
		MatchType(RadToken::StringConst) ||
		MatchType(RadToken::Number) ||
		MatchType(RadToken::DateConst) ||
		MatchType(RadToken::BoolConst) )
	{
		firstExpression = new PrimitiveExpression();
		((PrimitiveExpression*)firstExpression)->SetValue(*current);
		MoveNext();
	}
	else
	{
		//Nothing to do.
	}
	return firstExpression;
}

BaseExpression* RadParser::GetExpression()
{
	LogicalExpression* expression = GetLogicalExpression();
	if(!IsEnd() && Match(")"))
	{
		//MoveNext();
	}
	else if(IsEnd() ||  Match(";") || Match("=") || Match(",") || Match("}") || Match("]}")
		||Match("if") || Match("else") || Match("then") || Match("while")
		||Match("for") ||Match("to"))
	{
		//MoveNext();
	}
	else
	{
		throw RadSyntaxException("Expression end with error value",*current);
	}

	return expression;
}

BaseStatement* RadParser::GetStatement()
{
	if(IsEnd())
	{
		return NULL;
	}
	else if(MatchType(RadToken::Comments))
	{
		return GetCommentStatement();
	}
	else if(Match("if"))
	{
		return GetIfExpression();
	}
	else if(Match("while"))
	{
		return GetWhileExpression();
	}
	else if(Match("for"))
	{
		return GetForExpression();
	}
	else if(Match("(") && IsBracketStatements())
	{
		return GetBracketStatement();
	}
	else if(MatchType(RadToken::StringIndentifier) && IsCurrentSpecialFunction())
	{
		SpecialFunctionStatement* statement = new SpecialFunctionStatement();
		statement->SetContent(GetFunctionCallExpression());
		return statement;
	}
	else
	{
		return GetAssignmentExpression();
	}
}

ForExpression* RadParser::GetForExpression()
{
	ForExpression* forExpression = new ForExpression();
	if(MoveNext())
	{
		AssignmentExpression* forStart = GetAssignmentExpression();
		ArithmeticExpression* forEnd =NULL;
		BaseStatement* statement =NULL;
		if(Match("to"))
		{
			if(MoveNext())
			{
				forEnd= GetArithmeticExpression();
			}
			else
			{
				throw exception("For end expected in the end of file");
			}
			if(Match("do"))
			{
				MoveNext();
				statement = GetStatement();
			}
		}
		else
		{
			if(IsEnd())
			{
				throw exception("Key word 'to'expected in the end of file");
			}
			else
			{
				throw RadSyntaxException("Key word 'to'expected",*current);
			}
		}
		forExpression->SetForStart(forStart);
		forExpression->SetForEnd(forEnd);
		forExpression->SetContent(statement);
	}
	return forExpression;
}


CommentsStatement* RadParser::GetCommentStatement()
{
	CommentsStatement* statement = new CommentsStatement();
	statement->SetToken(*current);
	MoveNext();
	return statement;
}

IfExpression* RadParser::GetIfExpression()
{
	IfExpression*  expression = new IfExpression();

	if(MoveNext())
	{
		LogicalExpression* condition=GetLogicalExpression();
		BaseStatement* thenContent = NULL;
		BaseStatement* elsecontent = NULL;
		if(Match("then"))
		{
			MoveNext();
			thenContent = GetStatement();
			if(thenContent == NULL)
			{
				throw exception("There are no expressions after 'then'");
			}
		}
		else
		{
			if(IsEnd())
			{
				throw exception("Key word 'then' expected in the end of file");
			}
			else
			{
				throw RadSyntaxException("Key word 'then' expected",*current);
			}
		}
		if(!IsEnd() && Match("else"))
		{
			MoveNext();
			elsecontent = GetStatement();
			if(elsecontent == NULL)
			{
				throw exception("There are no expressions after 'else' in the end of file");
			}
		}
		expression->SetCondition(condition);
		expression->SetThenContent(thenContent);
		expression->SetElseContent(elsecontent);
	}

	return expression;
}

WhileExpression* RadParser::GetWhileExpression()
{
	WhileExpression*  expression = new WhileExpression();
	if(MoveNext())
	{
		LogicalExpression* condition=GetLogicalExpression();
		BaseStatement* content = NULL;
		if(Match("do"))
		{
			MoveNext();
			content = GetStatement();
		}
		else
		{
			if(IsEnd())
			{
				throw exception("Key word 'do' expected in the end of file");
			}
			else
			{
				throw RadSyntaxException("Key word 'do' expected",*current);
			}
		}
		expression->SetCondition(condition);
		expression->SetContent(content);
	}
	return expression;
}

/*
Gets Assignment expression.
*/
AssignmentExpression* RadParser::GetAssignmentExpression()
{
	AssignmentExpression* expression = new AssignmentExpression();
	BaseExpression* leftExpression = NULL;

	leftExpression = GetAssignmentLeft();

	expression->SetLeftExpression(leftExpression);

	if(!IsEnd())
	{
		if(Match("=") || Match("+=")||Match("-=")||Match("*=")||Match("/=")||Match("%="))
		{
			expression->SetSignToken(*current);
			if(MoveNext())
			{
				BaseExpression* rightExpression = GetAssignmentRightExpression();
				expression->SetRightExpression(rightExpression);
			}
			else
			{
				throw exception("Assignment right expected , in the end of file");
			}
		}
		else
		{
			if(IsEnd())
			{
				throw exception("Key word '=' expected , in the end of file");
			}
			else
			{
				throw RadSyntaxException("Key word '=' expected",*current);
			}
		}
	} 
	else
	{
		throw exception("Key word '=' expected in the end of file");
	}
	return expression;
}

BaseExpression* RadParser::GetAssignmentLeft()
{
	//BaseExpression* expression = NULL;
	//if(Match("("))
	//{
	//    expression = GetBracketExpression();
	//}
	//else if(MatchType(RadToken::Variable))
	//{
	//    list<RadToken>::iterator temIt = current;
	//    temIt++;
	//    if(temIt->GetTokenValue() == "in")
	//    {
	//        expression = GetInEndExpression();
	//    }
	//    else
	//    {
	//        expression = new PrimitiveExpression();
	//        RadToken token = *current;
	//        ((PrimitiveExpression*)expression)->SetValue(token);
	//        MoveNext();
	//    }
	//}
	//else
	//{
	//    expression = GetInEndExpression();
	//}
	//return expression;
	return GetInExpression();
}

BaseExpression* RadParser::GetAssignmentRightExpression()
{
	return GetExpression();
}

RadArray* RadParser::GetArray()
{
	RadArray* expression = NULL;
	if(Match("{"))
	{
		expression = new RadArray();
		MoveNext();

		list<BaseExpression*>* argsP = expression->GetArrayItemsP();
		FillCollectionItems(*argsP);
	}
	if(!Match("}"))
	{
		if(IsEnd())
		{
			throw exception("} expected, in the end of file");
		}
		else
		{
			throw RadSyntaxException("} expected",*current);
		}
	}
	if(!IsEnd())
	{
		MoveNext();
	}
	return expression;
}

RadStructure* RadParser::GetStructure()
{
	RadStructure* expression=NULL;
	if(Match("{["))
	{
		expression = new RadStructure();
		MoveNext();
		list<BaseExpression*>* argsP = expression->GetStructItemsP();
		FillCollectionItems(*argsP);
	}
	if(!Match("]}"))
	{
		if(IsEnd())
		{

			throw exception("]} expected, in the end of file");
		}
		else
		{
			throw RadSyntaxException("]} expected",*current);
		}
	}
	if(!IsEnd())
	{
		MoveNext();
	}
	return expression;
}

void RadParser::FillCollectionItems(list<BaseExpression*>& items)
{
	while(!IsEnd() && !Match("]}") && !Match("}")&&!Match(")"))
	{
		BaseExpression* expression = GetExpression();
		items.push_back(expression);
		if(Match(","))
		{
			MoveNext();
			if( Match("]}") || Match("}")||Match(")"))
			{
				items.push_back(NULL);
			}
		}
	}
}

/*
Get in expression.
Return value should be InExpression object
*/
InExpression* RadParser::GetInExpression()
{
	BaseExpression* inHead = GetInHeadExpression();
	BaseExpression* inEnd = GetInEndExpression();

	InExpression* expression = new InExpression();
	expression->SetHead(inHead);
	expression->SetEnd(inEnd);
	return expression;
}

BaseExpression* RadParser::GetInHeadExpression()
{
	return GetTermExpression();
}

InEndExpression* RadParser::GetInEndExpression()
{
	InEndExpression* expression = NULL;
	if(Match("in"))
	{
		expression = new InEndExpression();
		MoveNext();
		expression->SetValue( GetInExpression());
	}
	return expression;
}

CompareExpression* RadParser::GetCompareExpression()
{
	CompareExpression* expression = NULL;
	if(Match("not"))
	{
		MoveNext();
		expression = new LogicalNotExpression();
		((LogicalNotExpression*)expression)->SetValue(GetCompareExpression());
	}
	else
	{
		expression = new CompareExpression();

		InExpression* head = GetInExpression();
		CompareRightExpression* end = GetCompareRightExpression();
		expression->SetLeft(head);
		expression->SetRight(end);
	}

	return expression;
}

CompareRightExpression* RadParser::GetCompareRightExpression()
{
	CompareRightExpression* expression=NULL;
	if(Match("#") || Match("~#") ||Match("=") || Match("~=") || Match("<")||
		Match("<=") || Match(">") || Match(">=") || Match("isin"))
	{
		expression = new CompareRightExpression();
		expression ->SetSignToken(*current);
		MoveNext();
		expression->SetValue(GetCompareExpression());
	}
	return expression;
}

LogicalAndExpression* RadParser::GetLogicalAndExpression()
{
	LogicalAndExpression* expression = new LogicalAndExpression();
	expression->SetLeft(GetCompareExpression());
	expression->SetRight(GetLogicalAndRightExpression());
	return expression;
}

LogicalAndRightExpression* RadParser::GetLogicalAndRightExpression()
{
	LogicalAndRightExpression* expression = NULL;
	if(Match("and"))
	{
		expression = new LogicalAndRightExpression();
		MoveNext();
		expression->SetValue(GetLogicalAndExpression());
	}
	return expression;
}



ArithmeticLevelOneExpression* RadParser::GetArithmeticLevelOneExpression()
{
	ArithmeticLevelOneExpression* expression = new ArithmeticLevelOneExpression();
	expression->SetLeftSide(GetLogicalAndExpression());
	expression->SetRightSide(GetArithmeticLevelOneRightExpression());
	return expression;
}

ArithmeticLevelOneRightExpression* RadParser::GetArithmeticLevelOneRightExpression()
{ 
	ArithmeticLevelOneRightExpression* expression = NULL;

	if(Match("*") || Match("/") || Match("%"))
	{
		expression = new ArithmeticLevelOneRightExpression();
		expression->SetSignToken(*current);
		MoveNext();
		expression->SetValue(GetArithmeticLevelOneExpression());
	}
	return expression;
}


ArithmeticExpression* RadParser::GetArithmeticExpression()
{
	ArithmeticExpression* expression = new ArithmeticExpression();

	expression->SetLeftSide(GetArithmeticLevelOneExpression());
	expression->SetRightSide(GetArithmeticRightExpression());
	return expression;
}

ArithmeticRightExpression* RadParser::GetArithmeticRightExpression()
{
	ArithmeticRightExpression* expression =NULL;
	if(Match("+") || Match("-"))
	{ 
		expression = new ArithmeticRightExpression();
		expression->SetSignToken(*current);
		MoveNext();
		expression->SetValue(GetArithmeticExpression());
	}
	return expression;
}


LogicalExpression* RadParser::GetLogicalExpression()
{
	LogicalExpression* expression = new LogicalExpression();
	expression->SetLeft(GetArithmeticExpression());
	expression->SetRight(GetLogicalRightExpression());
	return expression;
}

LogicalRightExpression* RadParser::GetLogicalRightExpression()
{
	LogicalRightExpression* expression = NULL;

	if(Match("or"))
	{
		expression = new LogicalRightExpression();
		MoveNext();
		expression->SetValue(GetLogicalExpression());
	}
	return expression;
}

FunctionCallExpression* RadParser::GetFunctionCallExpression()
{
	FunctionCallExpression* expression = new FunctionCallExpression();
	if(MatchType(RadToken::StringIndentifier))
	{
		expression->SetFunctionName(current->GetTokenValue());
		MoveNext();
		if(!IsEnd())
		{
			if(Match("("))
			{
				MoveNext();
				if(!IsEnd())
				{
					list<BaseExpression*>* argsP = expression->GetFunctionArgsP();
					FillCollectionItems(*argsP);
				}
				if(!Match(")"))
				{
					if(IsEnd())
					{
						throw exception("Should be ')', in the end of file");
					}
					else
					{
						throw RadSyntaxException("Should be ')'",*current);
					}
				}
			}
		}
		else
		{
			throw exception("Should be '('");
		}
	}
	MoveNext();
	return expression;
}

BracketExpression* RadParser::GetBracketExpression()
{
	BracketExpression* bracketExpression=NULL;
	if(Match("("))
	{
		bracketExpression = new BracketExpression();
		MoveNext();
		BaseExpression* firstExpression = GetExpression();
		if(!IsEnd())
		{
			bracketExpression->SetValue(firstExpression);
		}
		else
		{
			throw exception("')' expected in the end of file");
		}


		if(!Match(")"))
		{
			if(IsEnd())
			{
				throw exception("')' expected, in the end of file");
			}
			else
			{
				throw RadSyntaxException("')' expected",*current);
			}
		}
		MoveNext();
	}
	return bracketExpression;
}


BracketStatement* RadParser::GetBracketStatement()
{
	BracketStatement* statement = new BracketStatement();
	if(Match("("))
	{
		bracketFlags.push(true);
		MoveNext();
		statement->SetValue(Parse());
		if(!Match(")"))
		{
			if(IsEnd())
			{

				throw exception("')' expected, in the end of file");
			}
			else
			{
				throw RadSyntaxException("')' expected here",*current);
			}
		}
		else
		{
			MoveNext();
		}
		bracketFlags.pop();
	}

	return statement;
}

void RadParser::ConstructSpecialFunctions()
{
	//Get the text file which stored the function names.
	TCHAR filePath[MAX_PATH];
	TCHAR path[MAX_PATH];
	DWORD dataLength = sizeof(path);

	TCHAR keyName[20];
	DWORD keyLength = _countof(keyName);
	const TCHAR g_szRegSubKey[] = TEXT("Software\\Benjamin\\Current Path");
	HKEY hkey;
	DWORD dwType;
	LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, g_szRegSubKey,0, KEY_QUERY_VALUE, &hkey);
	result = RegEnumValue(hkey, 0, keyName,&keyLength, NULL, &dwType, (PBYTE) path, &dataLength);
	keyName[keyLength] = TEXT('\0');
	wsprintf(filePath,TEXT("%s\\StandaloneFunctions.txt"),path);
	RegCloseKey(hkey);

	//Create the file base on the file name
	HANDLE file = CreateFile(filePath,GENERIC_READ,FILE_SHARE_READ ,NULL,OPEN_EXISTING,NULL,NULL);
	if(file != INVALID_HANDLE_VALUE)
	{
		DWORD fileSize = GetFileSize(file,NULL);
		char* buffer = new char[fileSize];
		DWORD readSize ;

		//Read all the text.
		ReadFile(file,buffer,fileSize,&readSize,NULL);

		//Get function names from the text.
		string functionName;
		for(int i=0;i<fileSize;i++)
		{
			if(buffer[i]!='\r' && buffer[i]!='\n')
			{
				functionName.push_back(buffer[i]);
			}
			else
			{
				if(functionName.length()>0)
				{
					specialFunctions.push_back(functionName);
					functionName.clear();
				}
			}
		}
		if(functionName.length()>0)
		{
			specialFunctions.push_back(functionName);
			functionName.clear();
		}

		delete[] buffer;
		CloseHandle(file);
	}
}

bool RadParser::IsCurrentSpecialFunction()
{
	list<RadToken>::iterator temIterator = current;
	string strName = current->GetTokenValue();
	temIterator++;

	if(temIterator == radTokens.end())
	{
		return false;
	}
	else 
	{
		string nextName = temIterator->GetTokenValue();
		if(nextName == "(")
		{
			for(int i=0;i<specialFunctions.size();i++)
			{
				if(specialFunctions.at(i) == strName)
				{
					return true;
				}
			}
		}
	}
	return false;
	//bool isFunctionCallStatement = false;
	//try
	//{
	//	GetFunctionCallExpression();

	//	if(IsEnd() || Match(string(";")))
	//	{
	//		current = temIterator;
	//		isFunctionCallStatement = true;
	//	}
	//}
	//catch(RadSyntaxException& e)
	//{

	//}
	//catch(exception& ex)
	//{
	//}
	//current = temIterator;
	//return isFunctionCallStatement;
}
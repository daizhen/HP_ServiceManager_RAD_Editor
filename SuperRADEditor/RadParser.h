#pragma once
#include "Statements.h"
#include "list"
#include "RadToken.h"
#include "IfExpression.h"
#include "WhileExpression.h"
#include "ForExpression.h"
#include "RadArray.h"
#include "RadStructure.h"
#include "FunctionCallExpression.h"
#include "InExpression.h"
#include "ArithmeticLevelOneExpression.h"
#include "ArithmeticLevelOneRightExpression.h"
#include "ArithmeticRightExpression.h"
#include "BaseStatement.h"
#include "CommentsStatement.h"

#include "BracketStatement.h"
#include "BracketExpression.h"
#include "LogicalRightExpression.h"
#include "InEndExpression.h"
#include "CompareRightExpression.h"
#include "LogicalAndRightExpression.h"
#include "vector"
#include "RadSyntaxException.h"
#include "stack"

using namespace std;

class RadParser
{
private:
    list<RadToken> radTokens ;
    list<RadToken>::iterator current;
    vector<string> specialFunctions;
	stack<bool> bracketFlags;
private:
     bool IsEnd();
     bool MoveNext();
     bool Match(string tokenValue);
     bool MatchType(RadToken::TokenType type);
     bool IsBracketStatements(/*list<RadToken>::iterator* iterator*/);
     
     bool IsCurrentSpecialFunction();
     BaseExpression* GetTermExpression();
     InExpression* GetInExpression();
     BaseExpression* GetInHeadExpression();
     InEndExpression* GetInEndExpression();

     CompareExpression* GetCompareExpression();
     CompareRightExpression* GetCompareRightExpression();

     LogicalAndExpression* GetLogicalAndExpression();
     LogicalAndRightExpression* GetLogicalAndRightExpression();
     
     ArithmeticLevelOneExpression* GetArithmeticLevelOneExpression();
     ArithmeticLevelOneRightExpression* GetArithmeticLevelOneRightExpression();

     ArithmeticExpression* GetArithmeticExpression();
     ArithmeticRightExpression* GetArithmeticRightExpression();

     LogicalExpression* GetLogicalExpression();
     LogicalRightExpression* GetLogicalRightExpression();

     BaseExpression* GetExpression();
     BaseStatement* GetStatement();
     IfExpression* GetIfExpression();
     WhileExpression* GetWhileExpression();
     ForExpression* GetForExpression();
	 CommentsStatement* GetCommentStatement();
     AssignmentExpression* GetAssignmentExpression();
     BaseExpression* GetAssignmentLeft();
     BracketStatement* GetBracketStatement();

     BaseExpression* GetAssignmentRightExpression();
     
     RadArray* GetArray();
     RadStructure* GetStructure();
     void FillCollectionItems(list<BaseExpression*>& items);
     FunctionCallExpression* GetFunctionCallExpression();
     BracketExpression* GetBracketExpression();

     void ConstructSpecialFunctions();

public:
     list<RadToken> GetRadTokens();
     Statements* Parse();

public:
    RadParser(list<RadToken> tokens);
    ~RadParser(void);
};


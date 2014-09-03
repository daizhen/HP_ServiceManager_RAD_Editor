#pragma once
#include "BaseExpression.h"
#include "LogicalExpression.h"

#include "Statements.h"
#include "BaseStatement.h"

class IfExpression: public BaseStatement
{
private:
    LogicalExpression* condition;
    BaseStatement* thenContent;
    BaseStatement* elseContent;
public :
     LogicalExpression* GetCondition();
     BaseStatement* GetThenContent();
     BaseStatement* GetElseContent();
     void SetCondition(LogicalExpression* condition);
     void SetThenContent(BaseStatement* thenStatement);
     void SetElseContent(BaseStatement* elseStatement);
     virtual string ToString(int nestLevel);
     virtual string ToString();
public:
    IfExpression(void);
    ~IfExpression(void);
};


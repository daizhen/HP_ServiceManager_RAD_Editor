#pragma once

#include "BaseExpression.h"
#include "LogicalExpression.h"
#include "Statements.h"
#include "BaseStatement.h"

class WhileExpression: public BaseStatement
{
private:
    LogicalExpression* condition;
    BaseStatement* content;

public:
    LogicalExpression* GetCondition();
    void SetCondition(LogicalExpression* condition)
    {
        this->condition = condition;
    }

    BaseStatement* GetContent();
    void SetContent(BaseStatement* value);
    virtual string ToString(int nestLevel);
    virtual string ToString();
public:
    WhileExpression(void);
    ~WhileExpression(void);
};


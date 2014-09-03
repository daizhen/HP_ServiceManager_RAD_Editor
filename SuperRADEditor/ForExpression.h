#pragma once

#include "BaseExpression.h"
#include "LogicalExpression.h"
#include "Statements.h"
#include "AssignmentExpression.h"
#include "ArithmeticExpression.h"
#include "BaseStatement.h"

class ForExpression: public BaseStatement
{
private:
    AssignmentExpression* forStart;
    ArithmeticExpression* forEnd;
    BaseStatement* content;

public:
    AssignmentExpression* GetForStart();
    ArithmeticExpression* GetForEnd();
    void SetForStart(AssignmentExpression* start);
    void SetForEnd(ArithmeticExpression* end);
    void SetContent(BaseStatement* statements);
    BaseStatement* GetContent();
    virtual string ToString(int nestLevel);
    virtual string ToString();

public:
    ForExpression(void);
    ~ForExpression(void);
};


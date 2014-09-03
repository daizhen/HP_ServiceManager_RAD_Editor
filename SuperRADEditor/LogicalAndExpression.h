#pragma once
#include "BaseExpression.h"
#include "CompareExpression.h"

class LogicalAndExpression : 
    public BaseExpression
{
private:
    CompareExpression* leftSide;
    BaseExpression* rightSide;

public:
    CompareExpression* GetLeft();
    BaseExpression* GetRight();
    void SetLeft(CompareExpression* leftValue);
    void SetRight(BaseExpression* rightValue);
    virtual string ToString();

public:
    LogicalAndExpression(void);
    ~LogicalAndExpression(void);
};
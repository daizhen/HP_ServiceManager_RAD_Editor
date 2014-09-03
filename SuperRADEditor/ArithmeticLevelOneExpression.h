#pragma once
#include "baseexpression.h"
#include "LogicalAndExpression.h"

class ArithmeticLevelOneExpression :
    public BaseExpression
{
private:
    LogicalAndExpression * leftSide;
    BaseExpression* rightSide;

public:
    LogicalAndExpression* GetLeftSide();
    BaseExpression* GetRightSide();

    void SetLeftSide( LogicalAndExpression* left);
    void SetRightSide( BaseExpression* right);
    virtual string ToString();

public:
    ArithmeticLevelOneExpression(void);
    ~ArithmeticLevelOneExpression(void);
};


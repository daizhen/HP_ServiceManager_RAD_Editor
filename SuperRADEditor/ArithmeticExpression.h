#pragma once
#include "BaseExpression.h"
#include "ArithmeticLevelOneExpression.h"

class ArithmeticExpression: public BaseExpression
{
private:
    ArithmeticLevelOneExpression * leftSide;
    BaseExpression* rightSide;

public:
    ArithmeticLevelOneExpression* GetLeftSide();
    BaseExpression* GetRightSide();

    void  SetLeftSide(ArithmeticLevelOneExpression* left);
    void SetRightSide (BaseExpression* right);

    virtual string ToString();

public:
    ArithmeticExpression(void);
    ~ArithmeticExpression(void);
};


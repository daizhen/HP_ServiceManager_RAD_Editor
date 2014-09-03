#pragma once
#include "BaseExpression.h"
#include "ArithmeticExpression.h"

class LogicalExpression : 
    public BaseExpression
{
private:
    ArithmeticExpression* left;
    BaseExpression* right;

public:
    
    ArithmeticExpression* GetLeft();

    BaseExpression* GetRight();

    void SetLeft(ArithmeticExpression* leftValue);

    void SetRight(BaseExpression* rightValue);

    virtual string ToString();
public:
    LogicalExpression(void);
    ~LogicalExpression(void);
};
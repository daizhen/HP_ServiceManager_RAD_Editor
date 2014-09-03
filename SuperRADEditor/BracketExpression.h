#pragma once
#include "baseexpression.h"

class BracketExpression :
    public BaseExpression
{
private :
    BaseExpression* value;
public:
    BaseExpression* GetValue()
    {
        return value;
    }
    void SetValue(BaseExpression* expression)
    {
        value = expression;
    }
public:
    BracketExpression(void);
    ~BracketExpression(void);
    virtual string ToString();
};


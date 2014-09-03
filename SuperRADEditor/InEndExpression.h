#pragma once
#include "BaseExpression.h"
#include "InExpression.h"

class InEndExpression:public BaseExpression
{
private :
    InExpression* value;

public:
    InExpression* GetValue()
    {
        return value;
    }

    void SetValue(InExpression* expression)
    {
        value = expression;
    }

    virtual string ToString();
public:
    InEndExpression(void);
    ~InEndExpression(void);
};


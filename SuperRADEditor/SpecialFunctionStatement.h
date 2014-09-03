#pragma once
#include "basestatement.h"
#include "FunctionCallExpression.h"

class SpecialFunctionStatement :
    public BaseStatement
{
private:
    FunctionCallExpression* content;
public:
    FunctionCallExpression* GetContent()
    {
        return content;
    }

    void SetContent (FunctionCallExpression* value)
    {
        content = value;
    }
    virtual string ToString(int nestLevel);
    virtual string ToString();

public:
    SpecialFunctionStatement(void);
    ~SpecialFunctionStatement(void);
};


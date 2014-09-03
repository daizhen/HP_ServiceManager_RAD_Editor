#pragma once
#include "BaseExpression.h"
#include "LogicalAndExpression.h"

class LogicalAndRightExpression : 
    public BaseExpression
{
private:
    LogicalAndExpression* value;

public:
    LogicalAndExpression* GetValue();
    void SetValue( LogicalAndExpression* expression);
    virtual string ToString();

public:
    LogicalAndRightExpression(void);
    ~LogicalAndRightExpression(void);
};

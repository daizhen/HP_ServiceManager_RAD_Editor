#pragma once
#include "BaseExpression.h"
#include "CompareExpression.h"

class LogicalNotExpression :
    public CompareExpression
{
private:
    CompareExpression* value;

public:
    CompareExpression* GetValue();

    void SetValue( CompareExpression*  exp);

    virtual string ToString();
public:
    LogicalNotExpression(void);
    ~LogicalNotExpression(void);
}; 
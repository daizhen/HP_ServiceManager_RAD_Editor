#pragma once
#include "BaseExpression.h"
#include "LogicalExpression.h"

class LogicalRightExpression 
    : public BaseExpression
{
private:
    LogicalExpression* value;

public:

    LogicalExpression* GetValue();
    void SetValue(LogicalExpression* expression);
    virtual string ToString();

public:
    LogicalRightExpression(void);
    ~LogicalRightExpression(void);
};

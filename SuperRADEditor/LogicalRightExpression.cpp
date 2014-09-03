#include "StdAfx.h"
#include "LogicalRightExpression.h"

LogicalRightExpression::LogicalRightExpression(void)
{
    value=NULL;
}

LogicalRightExpression::~LogicalRightExpression(void)
{
    if(value != NULL)
    {
        delete value;
    }
}

string LogicalRightExpression::ToString()
{
    string str;
    if(value !=NULL)
    {
		str.append("or ");
        str.append(value ->ToString());
    }
    return str;
}

LogicalExpression* LogicalRightExpression::GetValue()
{
    return value;
}

void LogicalRightExpression::SetValue(LogicalExpression* expression)
{
    value = expression;
}
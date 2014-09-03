#include "StdAfx.h"
#include "LogicalAndRightExpression.h"

LogicalAndRightExpression::LogicalAndRightExpression(void)
{
    value = NULL;
}


LogicalAndRightExpression::~LogicalAndRightExpression(void)
{
    if(value != NULL)
    {
        delete value;
    }
}

string LogicalAndRightExpression::ToString()
{
	string str;
	if(value!=NULL)
	{
		str.append("and ");
		str.append(value->ToString());
	}
	return str;
}


LogicalAndExpression* LogicalAndRightExpression::GetValue()
{
    return value;
}

void LogicalAndRightExpression::SetValue( LogicalAndExpression* rightValue)
{
    value = rightValue;
}
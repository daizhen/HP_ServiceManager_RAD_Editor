#include "StdAfx.h"
#include "LogicalNotExpression.h"

LogicalNotExpression::LogicalNotExpression(void)
{
}


LogicalNotExpression::~LogicalNotExpression(void)
{
}

string LogicalNotExpression::ToString()
{
    string str;

	if(value!= NULL)
	{
		str.append("not ");
		str.append(value->ToString());
	}
    return str;
}

CompareExpression* LogicalNotExpression::GetValue()
{
    return value;
}

void LogicalNotExpression::SetValue( CompareExpression*  exp)
{
    value = exp;
}
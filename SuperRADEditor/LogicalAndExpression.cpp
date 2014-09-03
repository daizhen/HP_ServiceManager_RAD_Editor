#include "StdAfx.h"
#include "LogicalAndExpression.h"

LogicalAndExpression::LogicalAndExpression(void)
{
    leftSide=NULL;
    rightSide = NULL;
}


LogicalAndExpression::~LogicalAndExpression(void)
{
    if(leftSide!=NULL)
    {
        delete leftSide;
    }

    if(rightSide!=NULL)
    {
        delete rightSide;
    }
}

string LogicalAndExpression::ToString()
{
    string str;
    if(leftSide!=NULL)
    {
        str.append(leftSide->ToString());
    }
    if(rightSide!=NULL)
    {
        str.append(" ");
        str.append(rightSide->ToString());
    }
    return str;
}

CompareExpression* LogicalAndExpression::GetLeft()
{
    return leftSide;
}
BaseExpression* LogicalAndExpression::GetRight()
{
    return rightSide;
}
void LogicalAndExpression::SetLeft(CompareExpression* leftValue)
{
    leftSide = leftValue;
}
void LogicalAndExpression::SetRight(BaseExpression* rightValue)
{
    rightSide = rightValue;
}
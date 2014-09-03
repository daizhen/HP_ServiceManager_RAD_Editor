#include "StdAfx.h"
#include "LogicalExpression.h"

LogicalExpression::LogicalExpression(void)
{
    left=NULL;
    right=NULL;
}


LogicalExpression::~LogicalExpression(void)
{
    if(left!=NULL)
    {
        delete left;
    } 
    if(right!=NULL)
    {
        delete right;
    }
}

string LogicalExpression::ToString()
{
    string str;
    if(left!=NULL)
    {
        str.append(left->ToString());
    }
    if(right!=NULL)
    {
		str.append(" ");
        str.append(right->ToString());
    }
    return str;
}

ArithmeticExpression* LogicalExpression::GetLeft()
{
    return left;
}

BaseExpression* LogicalExpression::GetRight()
{
    return right;
}

void LogicalExpression::SetLeft(ArithmeticExpression* leftValue)
{
    left = leftValue;
}

void LogicalExpression::SetRight(BaseExpression* rightValue)
{
    right = rightValue;
}
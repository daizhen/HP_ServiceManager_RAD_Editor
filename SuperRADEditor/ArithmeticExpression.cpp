#include "StdAfx.h"
#include "ArithmeticExpression.h"


ArithmeticExpression::ArithmeticExpression(void)
{
    leftSide=NULL;
    rightSide = NULL;
}

ArithmeticExpression::~ArithmeticExpression(void)
{
    if(leftSide != NULL)
    {
        delete leftSide;
    }
    if(rightSide != NULL)
    {
        delete rightSide;
    }
}

ArithmeticLevelOneExpression* ArithmeticExpression::GetLeftSide()
{
    return leftSide;
}
BaseExpression* ArithmeticExpression::GetRightSide()
{
    return rightSide;
}
void  ArithmeticExpression::SetLeftSide(ArithmeticLevelOneExpression* left)
{
    leftSide = left;
}

void ArithmeticExpression::SetRightSide (BaseExpression* right)
{
    rightSide = right;
}

string ArithmeticExpression::ToString()
{    
    string str;
    str.append(leftSide->ToString());
    if(rightSide!=NULL)
    {
		str.append(" ");
        str.append(rightSide->ToString());
    }
    return str;
}
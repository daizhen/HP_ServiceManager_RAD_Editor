#include "StdAfx.h"
#include "ArithmeticLevelOneExpression.h"


ArithmeticLevelOneExpression::ArithmeticLevelOneExpression(void)
{
}


ArithmeticLevelOneExpression::~ArithmeticLevelOneExpression(void)
{
}


LogicalAndExpression* ArithmeticLevelOneExpression::GetLeftSide()
{
    return leftSide;
}
BaseExpression* ArithmeticLevelOneExpression::GetRightSide()
{
    return rightSide;
}

void ArithmeticLevelOneExpression::SetLeftSide( LogicalAndExpression* left)
{
    leftSide = left;
}
void ArithmeticLevelOneExpression::SetRightSide( BaseExpression* right)
{
    rightSide = right;
}

string ArithmeticLevelOneExpression::ToString()
{
    string str;
    str.append(leftSide->ToString());
    if(rightSide!= NULL)
    {
		str.append(" ");
        str.append(rightSide->ToString());
    }
    return str;
}
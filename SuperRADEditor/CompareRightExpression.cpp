#include "StdAfx.h"
#include "CompareRightExpression.h"


CompareRightExpression::CompareRightExpression(void)
{
}


CompareRightExpression::~CompareRightExpression(void)
{
}


string CompareRightExpression::ToString()
{
    string str;
    str.append(signToken.GetTokenValue());
    if(value!=NULL)
    {
		str.append(" ");
        str.append(value->ToString());
    }
    return str;
}
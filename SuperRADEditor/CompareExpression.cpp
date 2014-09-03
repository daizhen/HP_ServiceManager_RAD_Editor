#include "StdAfx.h"
#include "CompareExpression.h"


CompareExpression::CompareExpression(void)
{
}


CompareExpression::~CompareExpression(void)
{
}


string CompareExpression::ToString()
{
    string str;
    str.append(left->ToString());
    if(right!=NULL)
    {
		str.append(" ");
        str.append(right->ToString());
    }
    return str;
}
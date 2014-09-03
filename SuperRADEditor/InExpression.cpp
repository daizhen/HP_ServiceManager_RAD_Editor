#include "StdAfx.h"
#include "InExpression.h"

InExpression::InExpression(void)
{
}


InExpression::~InExpression(void)
{
}

string InExpression::ToString()
{
    string str;
    if(head != NULL)
    {
        str.append(head->ToString());
    }

    if(end!=NULL)
    {
		str.append(" ");
        str.append(end->ToString());
    }
    return str;
}
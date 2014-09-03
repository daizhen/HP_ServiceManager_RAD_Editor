#include "StdAfx.h"
#include "InEndExpression.h"


InEndExpression::InEndExpression(void)
{
    value = NULL;
}


InEndExpression::~InEndExpression(void)
{
    if(value != NULL)
    {
        delete value;
    }
}

string InEndExpression::ToString()
{
    return "in "+ value->ToString(); 
}

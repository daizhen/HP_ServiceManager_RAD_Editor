#include "StdAfx.h"
#include "BracketExpression.h"


BracketExpression::BracketExpression(void)
{
}


BracketExpression::~BracketExpression(void)
{
}


 string BracketExpression::ToString()
 {
     string str;
     str.append("(");
     if(value!=NULL)
     {
         str.append(value->ToString());
     }
     str.append(")");
     return str;
 }
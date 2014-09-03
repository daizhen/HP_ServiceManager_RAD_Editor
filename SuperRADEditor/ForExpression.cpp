#include "StdAfx.h"
#include "ForExpression.h"


ForExpression::ForExpression(void)
{
	Type=For;
}


ForExpression::~ForExpression(void)
{
}

void ForExpression::SetForStart(AssignmentExpression* start)
{
    forStart = start;
}
void ForExpression::SetForEnd(ArithmeticExpression* end)
{
    forEnd = end;
}

void ForExpression::SetContent(BaseStatement* statement)
{
    content = statement;
}

BaseStatement* ForExpression::GetContent()
{
    return content;
}

string ForExpression::ToString(int nestLevel)
{
   string str;
   for(int i=0;i<nestLevel;i++)
   {
       str.append("    ");
   }
   str.append("for ");
   str.append(forStart->ToString(0));
   str.append(" to ");
   str.append(forEnd->ToString());
   str.append(" do");
   str.append("\r\n");
   str.append(content->ToString(nestLevel+1));
   return str;
}

string ForExpression::ToString()
{
   string str;
   str.append(" for ");
   str.append(forStart->ToString());
   str.append(" to ");
   str.append(forEnd->ToString());
   str.append(" do ");
   str.append(content->ToString());
   return str;
}
#include "StdAfx.h"
#include "IfExpression.h"


IfExpression::IfExpression(void)
{
	Type=If;
}


IfExpression::~IfExpression(void)
{
}

LogicalExpression* IfExpression::GetCondition()
{
    return condition;
}
BaseStatement* IfExpression::GetThenContent()
{
    return thenContent;
}
BaseStatement* IfExpression::GetElseContent()
{
    return elseContent;
}
void IfExpression::SetCondition(LogicalExpression* condition)
{
    this->condition = condition;
}
void IfExpression::SetThenContent(BaseStatement* thenStatement)
{
    thenContent  = thenStatement;
}
void IfExpression::SetElseContent(BaseStatement* elseStatement)
{
    elseContent = elseStatement;
}
string IfExpression::ToString(int nestLevel)
{
    string str;

    for(int i=0;i<nestLevel;i++)
    {
        str.append("    ");
    }
    str.append("if ");
    str.append(condition->ToString());
    str.append(" then");
    str.append("\r\n");
    str.append(thenContent->ToString(nestLevel+1));
    if(elseContent!=NULL)
    {
        str.append("\r\n");
        for(int i=0;i<nestLevel;i++)
        {
            str.append("    ");
        }
        str.append("else");
        str.append("\r\n");
        str.append(elseContent->ToString(nestLevel+1));
    }
    return str;
}


string IfExpression::ToString()
{
    string str;
    str.append("if ");
    str.append(condition->ToString());
    str.append(" then ");
    str.append(thenContent->ToString());
    if(elseContent!=NULL)
    {
        str.append(" else ");
        str.append(elseContent->ToString());
    }
    return str;
}
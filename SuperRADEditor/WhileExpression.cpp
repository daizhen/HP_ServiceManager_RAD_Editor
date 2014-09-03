#include "StdAfx.h"
#include "WhileExpression.h"


WhileExpression::WhileExpression(void)
{
	Type = While;
}

WhileExpression::~WhileExpression(void)
{
    if(condition!=NULL)
    {
        delete condition;
    }

    if(content!=NULL)
    {
        delete content;
    }
}


LogicalExpression* WhileExpression::GetCondition()
{
    return condition;
}

BaseStatement* WhileExpression::GetContent()
{
    return content;
}
void WhileExpression::SetContent(BaseStatement* value)
{
    content = value;
}

string WhileExpression::ToString(int nestLevel)
{
    string str;
    for(int i=0;i<nestLevel;i++)
    {
        str.append("    ");
    }
    str.append("while ");
    str.append(condition->ToString());
    str.append(" do");
    str.append("\r\n");
    str.append(content->ToString(nestLevel + 1));
    return str;
}
string WhileExpression::ToString()
{
    string str;
    str.append(" while ");
    str.append(condition->ToString());
    str.append(" do ");
    str.append(content->ToString());
    return str;
}
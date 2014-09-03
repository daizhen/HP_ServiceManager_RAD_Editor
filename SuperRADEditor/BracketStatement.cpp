#include "StdAfx.h"
#include "BracketStatement.h"


BracketStatement::BracketStatement(void)
{
}


BracketStatement::~BracketStatement(void)
{
}

string BracketStatement::ToString(int nestLevel)
{
    string temStr ;
    
    //Append header
    for(int i=0;i<nestLevel -1 ;i++)
    {
        temStr.append("    ");
    }
    temStr.append("(\r\n");
   
    //Append the value
    temStr.append(value->ToString(nestLevel));
    temStr.append("\r\n");
    //Append the footer.
    for(int i=0;i<nestLevel -1;i++)
    {
        temStr.append("    ");
    }
    temStr.append(")");
    return temStr;
}


string BracketStatement::ToString()
{
    string temStr ;
    temStr.append("( ");
   
    //Append the value
    temStr.append(value->ToString());
    temStr.append(" )");
    return temStr;
}
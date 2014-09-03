#include "StdAfx.h"
#include "SpecialFunctionStatement.h"


SpecialFunctionStatement::SpecialFunctionStatement(void)
{
	Type = FunctionCall;
}


SpecialFunctionStatement::~SpecialFunctionStatement(void)
{
}

string SpecialFunctionStatement::ToString(int nestLevel)
{
     string str;

    for(int i=0;i<nestLevel;i++)
    {
        str.append("    ");
    }
    if(content!=NULL)
    {
        str.append(content->ToString());
    }
    return str;
}

string SpecialFunctionStatement::ToString()
{
	string str;
	if(content!=NULL)
	{
		str.append(content->ToString());
	}
	return str;
}
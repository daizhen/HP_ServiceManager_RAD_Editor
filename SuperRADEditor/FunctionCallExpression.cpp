#include "StdAfx.h"
#include "FunctionCallExpression.h"


FunctionCallExpression::FunctionCallExpression(void)
{
}


FunctionCallExpression::~FunctionCallExpression(void)
{
}


void FunctionCallExpression::SetFunctionName(string name)
{
    functionName = name;
}


list<BaseExpression*> FunctionCallExpression::GetFunctionArgs()
{
    return functionArgs;
}


string FunctionCallExpression::GetFunctionName()
{
    return functionName;
}

list<BaseExpression*>* FunctionCallExpression::GetFunctionArgsP()
{
    return &functionArgs;
}

string FunctionCallExpression::ToString()
{
    string str;
    str.append(functionName);
    str.append("(");

    int argIndex =0;
    int argCount = functionArgs.size();

    for(list<BaseExpression*>::iterator currentIterator = functionArgs.begin();currentIterator!=functionArgs.end();currentIterator++)
    {
        if(*currentIterator!=NULL)
        {
            str.append((*currentIterator)->ToString());
        }
        else
        {
             str.append(" ");
        }
        if(argIndex < argCount-1)
        {
             str.append(",");
        }
        argIndex++;
    }
    str.append(")");
    return str;
}
#include "StdAfx.h"
#include "RadArray.h"


RadArray::RadArray(void)
{
}


RadArray::~RadArray(void)
{
}

list<BaseExpression*> RadArray::GetArrayItems()
{
    return arrayItems;
}

list<BaseExpression*>* RadArray::GetArrayItemsP()
{
    return &arrayItems;
}

string RadArray::ToString()
{
    string str;
    str.append("{");

    int argIndex =0;
    int argCount = arrayItems.size();

    for(list<BaseExpression*>::iterator currentIterator = arrayItems.begin();currentIterator!=arrayItems.end();currentIterator++)
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
    str.append("}");
    return str;
}
#include "StdAfx.h"
#include "RadStructure.h"


RadStructure::RadStructure(void)
{
}


RadStructure::~RadStructure(void)
{
}

 list<BaseExpression*> RadStructure::GetStructItems()
 {
     return structItems;
 }

 list<BaseExpression*>* RadStructure::GetStructItemsP()
 {
     return &structItems;
 }

 string RadStructure::ToString()
 {
     string str;
     str.append("{[");
     int argIndex =0;
     int argCount = structItems.size();

     for(list<BaseExpression*>::iterator currentIterator = structItems.begin();currentIterator!=structItems.end();currentIterator++)
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
     str.append("]}");
     return str;
 }
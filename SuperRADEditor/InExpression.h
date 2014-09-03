#pragma once
#include "BaseExpression.h"

class InExpression:
    public BaseExpression
{
private:
    BaseExpression* head;

    //This should be InEnd object.
    BaseExpression* end;

public:
     BaseExpression* GetHead() 
     {
         return head;
     }

     void SetHead(BaseExpression* headValue)
     {
         head = headValue;
     }

     BaseExpression* GetEnd() 
     {
         return end;
     }

     void SetEnd(BaseExpression* endValue)
     {
         end = endValue;
     }
     virtual string ToString();
public:
    InExpression(void);
    ~InExpression(void);
};


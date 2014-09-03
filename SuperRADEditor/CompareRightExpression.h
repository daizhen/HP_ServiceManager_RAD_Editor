#pragma once
#include "baseexpression.h"
#include "RadToken.h"
#include "CompareExpression.h"

class CompareRightExpression :
    public BaseExpression
{
private:
    RadToken signToken;
    CompareExpression* value;

public:
    
     RadToken GetSignToken()
     {
         return signToken;
     }
     void SetSignToken(RadToken& token)
     {
         signToken = token;
     }
     CompareExpression* GetValue()
     {
         return value;
     }
     void SetValue(CompareExpression* expression)
     {
         value = expression;
     }
     virtual string ToString();

public:
    CompareRightExpression(void);
    ~CompareRightExpression(void);
};


#pragma once
#include "BaseExpression.h"
#include "RadToken.h"

class PrimitiveExpression :
    public BaseExpression
{
private :
    RadToken token;
public:
    RadToken GetToken();
    void SetValue(RadToken& value);
    virtual string ToString();
public:
    PrimitiveExpression(void);
    ~PrimitiveExpression(void);
};


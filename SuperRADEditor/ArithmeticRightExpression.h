#pragma once
#include "baseexpression.h"
#include "RadToken.h"
#include "ArithmeticExpression.h"

class ArithmeticRightExpression :
    public BaseExpression
{
private :
    RadToken signToken;
    ArithmeticExpression* value;
public:

    RadToken GetSignToken();
    void SetSignToken(RadToken& token);

    ArithmeticExpression* GetValue();
    void SetValue (ArithmeticExpression* expression);

    virtual string ToString();
public:
    ArithmeticRightExpression(void);
    ~ArithmeticRightExpression(void);
};


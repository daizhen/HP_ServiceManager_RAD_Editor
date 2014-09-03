#pragma once
#include "baseexpression.h"
#include "RadToken.h"
#include "ArithmeticLevelOneExpression.h"

class ArithmeticLevelOneRightExpression :
    public BaseExpression
{
    
private:
    ArithmeticLevelOneExpression* value;
    RadToken signToken;
public:
    bool IsNull;
    ArithmeticLevelOneExpression* GetValue();
    void SetValue( ArithmeticLevelOneExpression* expression);

    RadToken GetSignToken();
    void SetSignToken(RadToken& token);

    virtual string ToString();

public:
    ArithmeticLevelOneRightExpression(void);
    ~ArithmeticLevelOneRightExpression(void);
};


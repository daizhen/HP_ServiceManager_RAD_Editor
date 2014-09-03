#pragma once
#include "BaseExpression.h"
#include "InExpression.h"

class CompareExpression :
    public BaseExpression
{
private:
    InExpression* left;
    
    //Should be CompareRightExpression
    BaseExpression* right;
public:

    InExpression* GetLeft()
    {
        return left;
    }
    BaseExpression* GetRight()
    {
        return right;
    }
    void SetLeft(InExpression* leftValue)
    {
        left = leftValue;
    }
    void SetRight( BaseExpression* rightValue)
    {
        right = rightValue;
    }

     virtual string ToString();

public:
    CompareExpression(void);
    ~CompareExpression(void);
};


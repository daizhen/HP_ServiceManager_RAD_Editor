#pragma once
#include "string"

using namespace std;

/*
    The base expression for other expressions.
*/
class BaseExpression
{
public:
    enum ExpressionType 
    {
        In,
        Logical,
        LogicalRight,
        LogicalLevelTwo,
        LogicalLevelTwoRight,
        Arithmetic,
        ArithmeticLevelTwo,
        ArithmeticRight,
        ArithmeticLevelTwoRight,

        NullType,
        Number,
        StringConst,
        DateConst,
        Variable,
        Primitive,
        StringIdentifier
    }
    Type;
public:

    //Used to indicate whether the statements is quoted with brackets.
    bool HasBrackets;

    BaseExpression(void);
    virtual ~BaseExpression(void);
    virtual string ToString();
};


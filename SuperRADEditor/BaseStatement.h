#pragma once

#include "string"
using namespace std;

class BaseStatement
{
public:
    enum StatementType 
    {
        Assignment=0,
        If=1,
        For=2,
        While=3,
		Comments=4,
        NullType=5,
		FunctionCall = 6
    }
    Type;
public:
    BaseStatement(void);
    ~BaseStatement(void);
    virtual string ToString(int nestLevel);
    virtual string ToString();

};


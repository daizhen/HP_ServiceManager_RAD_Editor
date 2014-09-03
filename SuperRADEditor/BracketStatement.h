#pragma once
#include "basestatement.h"
class BracketStatement :
    public BaseStatement
{
private: 
    BaseStatement* value;

public:
    BaseStatement* GetValue()
    {
        return value;
    }
    void SetValue(BaseStatement* statement)
    {
        value = statement;
    }
public:
    BracketStatement(void);
    ~BracketStatement(void);
    virtual string ToString(int nestLevel);
    virtual string ToString();
};


#pragma once
#include "string"
#include "list"
#include "BaseExpression.h"

using namespace std;

class FunctionCallExpression: public BaseExpression
{
private :
    string functionName;
    list<BaseExpression*> functionArgs;
public:
    string GetFunctionName();
    void SetFunctionName(string name);
    list<BaseExpression*> GetFunctionArgs();
    list<BaseExpression*>* GetFunctionArgsP();
    virtual string ToString();

public:
    FunctionCallExpression(void);
    ~FunctionCallExpression(void);
};


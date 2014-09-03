#pragma once
#include "BaseExpression.h"
#include "list"
#include "BaseStatement.h"

using namespace std;
class Statements:public BaseStatement
{
private:
    list<BaseStatement*> content;
public:
    list<BaseStatement*> GetContent();
    void AddStatement(BaseStatement* item);
    virtual string ToString(int nestLevel);
    virtual string ToString();
public:
    Statements(void);
    ~Statements(void);
};
#pragma once
#include "BaseExpression.h"
#include "list"
using namespace std;

class RadStructure: public BaseExpression
{
private:
    list<BaseExpression*> structItems;
public:
    list<BaseExpression*> GetStructItems();
    list<BaseExpression*>* GetStructItemsP();
    virtual string ToString();
public:
    RadStructure(void);
    ~RadStructure(void);
};


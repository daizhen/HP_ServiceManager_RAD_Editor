#pragma once
#include "BaseExpression.h"
#include "list"
using namespace std;

class RadArray: public BaseExpression
{
private:

    list<BaseExpression*> arrayItems;

public:
    list<BaseExpression*> GetArrayItems();
    list<BaseExpression*>* GetArrayItemsP();

    virtual string ToString();

public:
    RadArray(void);
    ~RadArray(void);
};


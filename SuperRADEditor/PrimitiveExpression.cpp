#include "StdAfx.h"
#include "PrimitiveExpression.h"


PrimitiveExpression::PrimitiveExpression(void)
{
}


PrimitiveExpression::~PrimitiveExpression(void)
{
}

RadToken PrimitiveExpression::GetToken()
{
    return token;
}
void PrimitiveExpression::SetValue(RadToken& value)
{
    token = value;
}

string PrimitiveExpression::ToString()
{
    return token.GetTokenValue();
}
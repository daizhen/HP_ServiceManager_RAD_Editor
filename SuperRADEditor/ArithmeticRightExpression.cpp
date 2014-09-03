#include "StdAfx.h"
#include "ArithmeticRightExpression.h"


ArithmeticRightExpression::ArithmeticRightExpression(void)
{
}


ArithmeticRightExpression::~ArithmeticRightExpression(void)
{
}



void ArithmeticRightExpression::SetSignToken(RadToken& token)
{
    signToken = token;
}

RadToken ArithmeticRightExpression::GetSignToken()
{
    return signToken;
}


void  ArithmeticRightExpression::SetValue(ArithmeticExpression* expression)
{
    value = expression;
}
ArithmeticExpression* ArithmeticRightExpression::GetValue()
{
    return value;
}

string ArithmeticRightExpression::ToString()
{
    string str;
	if(value != NULL)
	{
		str.append(signToken.GetTokenValue());
		str.append(" ");
		str.append(value->ToString());
	}
	return str;
}
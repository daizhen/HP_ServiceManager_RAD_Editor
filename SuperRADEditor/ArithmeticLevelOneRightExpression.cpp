#include "StdAfx.h"
#include "ArithmeticLevelOneRightExpression.h"


ArithmeticLevelOneRightExpression::ArithmeticLevelOneRightExpression(void)
{
}


ArithmeticLevelOneRightExpression::~ArithmeticLevelOneRightExpression(void)
{
}

ArithmeticLevelOneExpression* ArithmeticLevelOneRightExpression::GetValue()
{
    return value;
}

void ArithmeticLevelOneRightExpression::SetValue( ArithmeticLevelOneExpression* expression)
{
    value = expression;
}

RadToken ArithmeticLevelOneRightExpression::GetSignToken()
{
    return signToken;
}
void ArithmeticLevelOneRightExpression::SetSignToken(RadToken& token)
{
    signToken = token;
}

string ArithmeticLevelOneRightExpression::ToString()
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
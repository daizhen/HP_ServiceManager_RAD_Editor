#pragma once
#include "BaseExpression.h"
#include "BaseStatement.h"
#include "RadToken.h"

class AssignmentExpression: public BaseStatement
{
private:
	BaseExpression* leftExpression;
	RadToken signToken;
	BaseExpression* rightExpression;


public :
	BaseExpression* GetLeftExpression()
	{
		return leftExpression;
	}
	BaseExpression* GetRightExpression()
	{
		return rightExpression;
	}
	void SetLeftExpression(BaseExpression* expression)
	{
		leftExpression = expression;
	}
	void SetRightExpression(BaseExpression* expression)
	{
		rightExpression = expression;
	}

	RadToken GetSignToken()
	{
		return signToken;
	}

	void SetSignToken(RadToken token)
	{
		signToken = token;
	}
	virtual string ToString(int nestLevel);
	virtual string ToString();
public:
	AssignmentExpression(void);
	~AssignmentExpression(void);
};


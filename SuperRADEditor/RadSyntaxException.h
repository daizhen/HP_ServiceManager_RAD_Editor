#pragma once
#include <exception>
#include "RadToken.h"

using namespace std;

class RadSyntaxException :
	public exception
{
private :
	int line;
	int positionOfLine;
	
public:
	RadSyntaxException(const char * exceptionMessage,int exceptionLine, int column);
	RadSyntaxException(const char * exceptionMessage,RadToken token);
	~RadSyntaxException(void);
	virtual const char* what() const;
};

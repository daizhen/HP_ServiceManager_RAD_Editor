#include "StdAfx.h"
#include "RadSyntaxException.h"
#include <string>

RadSyntaxException::RadSyntaxException(const char * exceptionMessage,int exceptionLine, int column):
exception(exceptionMessage)
{
	line=exceptionLine;
	positionOfLine = column;
}

RadSyntaxException::RadSyntaxException(const char * exceptionMessage,RadToken token)
:exception(exceptionMessage)
{
	line=token.GetLine();
	positionOfLine = token.GetPositionOfLine();
}
const char* RadSyntaxException::what() const
{
	char temNumber[33];
	static string exceptionMessage;

	exceptionMessage.clear();

	exceptionMessage.append( exception::what());
	exceptionMessage.append("  line:");
	sprintf(temNumber,"%d",line);
	exceptionMessage.append(temNumber);
	//exceptionMessage+=line;
	exceptionMessage.append(",  column:");

	sprintf(temNumber,"%d",positionOfLine);
	exceptionMessage.append(temNumber);

	//exceptionMessage+=positionOfLine;
	return exceptionMessage.c_str();
}


RadSyntaxException::~RadSyntaxException(void)
{

}
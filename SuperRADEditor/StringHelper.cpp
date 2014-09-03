#include "StdAfx.h"
#include "StringHelper.h"

StringHelper::StringHelper(void)
{
}

StringHelper::~StringHelper(void)
{
}


string StringHelper::EscapeString(string str)
{
	string resultString ="";
	for(int i=0;i<str.size();i++)
	{
		char currentChar = str[i];
		if(currentChar=='"' || currentChar=='\\')
		{
			resultString.append("\\");
		}
		resultString.push_back(currentChar);
	}
	return resultString;
}

string StringHelper::UnescapeString(string str)
{
    string resultString;

    //Used to check whether the last char is escape char. that means \

    bool isLastEscapeChar= false;
    bool isSuccess = true;
    for(int i=1;i<str.size()-1;i++)
    {
        char currentChar = str[i];
        if(currentChar == '"')
        {
            if(!isLastEscapeChar)
            {
                isSuccess = false;
                break;
            }
            else
            {
				resultString.push_back(currentChar);
                isLastEscapeChar = false;
            }
        }
        else
        {
            if(currentChar == '\\' )
            {
				if(!isLastEscapeChar)
				{
					isLastEscapeChar = true;
				}
				else
				{
					isLastEscapeChar = false;
					resultString.push_back(currentChar);
				}
            }
            else
            {
				resultString.push_back(currentChar);
                isLastEscapeChar = false;
            }
        }
    }

    //if(!isSuccess)
    //{
    //    throw exception("String const  not valid");
    //}
    return resultString;
}

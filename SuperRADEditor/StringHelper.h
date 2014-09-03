#pragma once

#include "string"
using namespace std;
class StringHelper
{
public:
	static string EscapeString(string str);
	static string UnescapeString(string str);

	StringHelper(void);
	~StringHelper(void);
};

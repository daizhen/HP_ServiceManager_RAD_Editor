#pragma once
#include "basestatement.h"
#include "RadToken.h"

class CommentsStatement :
	public BaseStatement
{
private :
	RadToken token;

public:
	CommentsStatement(void);
	~CommentsStatement(void);

public :

	RadToken GetToken();
	void SetToken(RadToken& radToken);
    virtual string ToString(int nestLevel);
    virtual string ToString();
};

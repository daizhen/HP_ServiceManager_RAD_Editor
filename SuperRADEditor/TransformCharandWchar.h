class TransformCharandWchar
{
private:
	char *m_pChar;
	WCHAR *m_pWchar;
public:
	TransformCharandWchar():m_pChar(NULL),m_pWchar(NULL)
	{}
	~TransformCharandWchar()
	{
		if (m_pChar)
		{
			delete m_pChar;
		}
		if (m_pWchar)
		{
			delete m_pWchar;
		}
	}
	const char * WcharTochar(const WCHAR* pwchar)
	{
		if(pwchar == NULL)
			return NULL;
		if(m_pChar!=NULL)
			delete m_pChar;
		int outlen = WideCharToMultiByte(CP_ACP,0,pwchar,-1,NULL,0,NULL,0);
		m_pChar = new char[outlen];
		WideCharToMultiByte(CP_ACP,0,pwchar,-1,m_pChar,outlen,NULL,0);
		return m_pChar;
	}
	const WCHAR* CharToWchar(const char * pchar)
	{
		if(pchar == NULL)
			return NULL;
		if(m_pWchar!=NULL)
			delete m_pWchar;
		int outlen = MultiByteToWideChar(CP_ACP,0,pchar,-1,NULL,0);
		m_pWchar = new WCHAR[outlen];
		MultiByteToWideChar(CP_ACP,0,pchar,-1,m_pWchar,outlen);
		return m_pWchar;
	}
};
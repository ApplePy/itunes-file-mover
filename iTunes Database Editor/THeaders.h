#pragma once
#include <tchar.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace std
{
	typedef basic_string<TCHAR>         tstring;

	typedef basic_ostream<TCHAR>        tostream;
	typedef basic_istream<TCHAR>        tistream;
	typedef basic_iostream<TCHAR>       tiostream;

	typedef basic_ifstream<TCHAR>       tifstream;
	typedef basic_ofstream<TCHAR>       tofstream;
	typedef basic_fstream<TCHAR>        tfstream;

	typedef basic_stringstream<TCHAR>   tstringstream;

	extern tostream& tcout;

	template <class T>
	inline tstring to_tstring(T _Val)
	{	
#if defined(UNICODE) || defined(_UNICODE)
		return to_wstring(_Val);
#else
		return to_string(_Val);
#endif
	}
}

// Source: http://www.rioki.org/2010/09/24/tstring.html

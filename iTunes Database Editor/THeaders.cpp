#include <iostream>

namespace std
{
#if defined(UNICODE) || defined(_UNICODE)
	wostream& tcout = wcout;
#else
	ostream& tcout = cout;
#endif
}
#include <iostream>
#include "THeaders.h"

namespace std
{
#if defined(UNICODE) || defined(_UNICODE)
	tostream& tcout = wcout;
#else
#
	tostream& tcout = cout;
#endif
}
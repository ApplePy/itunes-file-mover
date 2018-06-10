#pragma once
#include <string>

BSTR ConvertMBSToBSTR(const std::string& str);
std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen);
std::string ConvertBSTRToMBS(BSTR bstr);


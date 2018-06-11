#include "stdafx.h"
#include "ConversionHelpers.h"

/// <summary> Converts from C/C++ multi-byte strings to wide strings. </summary>
/// <param name="mbs"> Multi-byte string to convert. </param>
/// <returns> The wide string. </returns>
std::wstring ConvertMBSToWCS(const std::string& mbs)
{
	// Get length of string
	int wideStringLength = MultiByteToWideChar(CP_UTF8, 0 /* no flags */,
		mbs.data(), mbs.length(), NULL, 0);

	// Allocate space for the BSTR string and copy characters over
	std::wstring wcs(wideStringLength, TCHAR('\0'));

	MultiByteToWideChar(CP_UTF8, 0 /* no flags */,
		mbs.data(), mbs.length(), &wcs[0], wideStringLength);

	return wcs;
}


/// <summary> Converts from wide strings to multi-byte strings. </summary>
/// <param name="wcs"> The wide string to convert. </param>
/// <returns> The multi-byte string. </returns>
std::string ConvertWCSToMBS(const std::wstring& wcs)
{
	// Get length of string
	int len = WideCharToMultiByte(CP_UTF8, 0 /* no flags */,
		wcs.data(), wcs.length(), NULL, 0, NULL, NULL);

	// Allocate space for string
	std::string mbs(len, '\0');

	// Convert wide string
	len = WideCharToMultiByte(CP_UTF8, 0 /* no flags */,
		&wcs[0], wcs.length() /* not necessary NULL-terminated */,
		&mbs[0], len,
		NULL, NULL /* no default char */);

	return mbs;
}


/// <summary> Converts a tstring into std::string. </summary>
/// <param name="str"> The tstring to convert. </param>
/// <returns> a std::string </returns>
std::string ConvertToMBS(const std::tstring& str)
{
#ifdef _UNICODE
	std::string mbsString(ConvertWCSToMBS(str));
#else
	std::string mbsString(str);
#endif
	return mbsString;
}


/// <summary> Converts a tstring into std::wstring. </summary>
/// <param name="str"> The tstring to convert. </param>
/// <returns> a std::wstring </returns>
std::wstring ConvertToWCS(const std::tstring& str)
{
#ifndef _UNICODE
	std::wstring wcsString(ConvertMBSToWCS(str));
#else
	std::wstring wcsString(str);
#endif
	return wcsString;
}


/// <summary> Handles checking for success of a COM call, and throws an std::runtime_error if it fails. </summary>
/// <param name="callResult"> The result to check. </param>
/// <param name="errExplanation"> Custom text to use for the error explanation. </param>
void HandleCOMErrors(HRESULT callResult, const std::tstring errExplanation)
{
	if (callResult != S_OK)
	{
		std::tstringstream ss;
		ss << errExplanation << callResult;
		throw std::runtime_error(ConvertToMBS(ss.str().data()));
	}
}

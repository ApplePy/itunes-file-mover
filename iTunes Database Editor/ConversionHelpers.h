#pragma once
#include "stdafx.h"

/// <summary> Converts from C/C++ multi-byte strings to wide strings. </summary>
/// <param name="mbs"> Multi-byte string to convert. </param>
/// <returns> The wide string. </returns>
std::wstring ConvertMBSToWCS(const std::string& mbs);


/// <summary> Converts from wide strings to multi-byte strings. </summary>
/// <param name="wcs"> The wide string to convert. </param>
/// <returns> The multi-byte string. </returns>
std::string ConvertWCSToMBS(const std::wstring& wcs);


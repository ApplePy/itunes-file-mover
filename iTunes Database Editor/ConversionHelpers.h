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


/// <summary> Handles checking for success of a COM call, and throws an std::runtime_error if it fails. </summary>
/// <param name="callResult"> The result to check. </param>
/// <param name="errText"> Custom text to use for the error explanation. </param>
void HandleCOMErrors(HRESULT callResult, std::string errText = "Instantiation of an iTunes COM object failed. Result was: ")
{
	if (callResult != S_OK)
	{
		std::stringstream ss;
		ss << errText << callResult;
		throw std::runtime_error(ss.str().data());
	}
}


/// <summary> Ensures safe deletion of a COM object pointer. </summary>
template <class T>
void SafeDeleteCOMObject(T* obj)
{
	if (obj)
	{
		obj->Release();
		obj = NULL;
	}
}


/// <summary> Creates an std::unique_ptr out of raw pointers, and erases the old raw pointer </summary>
template <class T>
std::unique_ptr<T, std::function<void(T*)>> WrapRawPtr(T*& ptr)
{
	auto smartPtr = std::unique_ptr<T, std::function<void(T*)>>(ptr, SafeDeleteCOMObject<T>);
	ptr = NULL;
	return smartPtr;
}
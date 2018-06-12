#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <functional>
#include <memory>
#include "THeaders.h"

template <class T>
using ComPtr = std::unique_ptr<T, std::function<void(T*)>>;

namespace iTunesHelpers
{

	/// <summary> Converts from C/C++ multi-byte strings to wide strings. </summary>
	/// <param name="mbs"> Multi-byte string to convert. </param>
	/// <returns> The wide string. </returns>
	std::wstring ConvertMBSToWCS(const std::string& mbs);


	/// <summary> Converts from wide strings to multi-byte strings. </summary>
	/// <param name="wcs"> The wide string to convert. </param>
	/// <returns> The multi-byte string. </returns>
	std::string ConvertWCSToMBS(const std::wstring& wcs);


	/// <summary> Converts a tstring into std::string. </summary>
	/// <param name="str"> The tstring to convert. </param>
	/// <returns> a std::string </returns>
	std::string ConvertToMBS(const std::tstring& str);


	/// <summary> Converts a tstring into std::wstring. </summary>
	/// <param name="str"> The tstring to convert. </param>
	/// <returns> a std::wstring </returns>
	std::wstring ConvertToWCS(const std::tstring& str);


	/// <summary> Converts a std::wstring into a tstring. </summary>
	/// <param name="str"> The wstring to convert. </param>
	/// <returns> a std::tstring </returns>
	std::tstring ConvertToTS(const std::wstring& str);


	/// <summary> Converts a std::string into a tstring. </summary>
	/// <param name="str"> The std::string to convert. </param>
	/// <returns> a std::tstring </returns>
	std::tstring ConvertToTS(const std::string& str);


	/// <summary> Handles checking for success of a COM call, and throws an std::runtime_error if it fails. </summary>
	/// <param name="callResult"> The result to check. </param>
	/// <param name="errExplanation"> Custom text to use for the error explanation. </param>
	void HandleCOMErrors(HRESULT callResult, const std::tstring errExplanation = _T("Instantiation of an iTunes COM object failed. Result was: "));


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
	ComPtr<T> WrapRawPtr(T*& ptr)
	{
		auto smartPtr = ComPtr<T>(ptr, SafeDeleteCOMObject<T>);
		ptr = NULL;
		return smartPtr;
	}


	template <class T>
	ComPtr<T> GetNewInterface(IUnknown* COMObject, const IID requestedInterface)
	{
		T* holder;
		COMObject->QueryInterface(requestedInterface, (void**)&holder);

		if (holder == NULL) throw std::invalid_argument("COM object does not support this interface.");

		return WrapRawPtr(holder);
	}
}
#include <iostream>
#include <combaseapi.h>
#include "ConversionHelpers.h"
#include "iTunesOperations.h"

using namespace iTunesHelpers;

iTunesOperations::iTunesOperations()
{
	// Get COM started
	HRESULT comInitRes = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (comInitRes != S_OK && comInitRes != S_FALSE)
	{
		std::tstringstream ss;
		ss << _T("Inititalization of COM library failed. Result was: ") << comInitRes;
		throw std::runtime_error(ConvertToMBS(ss.str().data()));
	}

	// Get the iTunes library object
	IiTunes* iTunesTmpRawPtr;
	HandleCOMErrors(
		CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, reinterpret_cast<PVOID *>(&iTunesTmpRawPtr)),
		_T("Instantiation of iTunes COM interface failed. Result was: "));

	// Move the raw pointer into a smart pointer, and give it a safe-delete function.
	iTunes = ComPtr<IiTunes>(iTunesTmpRawPtr, SafeDeleteCOMObject<IiTunes>);
	iTunesTmpRawPtr = NULL;
}

void iTunesOperations::moveTrack(std::tstring sourcePrefix, std::tstring destinationPrefix, const trackPtr& track)
{
	ComPtr<IITFileOrCDTrack> trackFileObject;
	auto location = GetFileName(track, &trackFileObject);

	if (location.length() == 0 || location.find(sourcePrefix) != 0) return;

#if defined(_DEBUG) || defined(DEBUG)
	std::tcout << location.c_str() << std::endl;
#endif

	location.replace(0, sourcePrefix.length(), destinationPrefix);

#if defined(_DEBUG) || defined(DEBUG)
	std::tcout << "Would rewrite to: " << location.c_str() << std::endl;
#else
	BSTR locationCOM = const_cast<wchar_t*>(ConvertToWCS(location).data());
	HandleCOMErrors(trackFileObject->put_Location(locationCOM));
#endif
}

iTunesOperations::~iTunesOperations()
{
	// Close up COM
	iTunes.reset();	// In case of reversed order (e.g. COM is closed before the object is released)
	CoUninitialize();
}

std::tstring iTunesOperations::GetFileName(const trackPtr& track, ComPtr<IITFileOrCDTrack>* fileObject)
{
	ITTrackKind trackType;
	HandleCOMErrors(track->get_Kind(&trackType));

	if (trackType != ITTrackKindFile) return std::tstring();

	// Convert the track type into a new type that contains the file path (since some tracks have no file, i.e. internet streams)
	auto fileTrack = GetNewInterface<IITFileOrCDTrack>(static_cast<IUnknown*>(track.get()), IID_IITFileOrCDTrack);

	BSTR locationRaw = NULL;
	HandleCOMErrors(fileTrack->get_Location(&locationRaw));
	auto location = ConvertToTS(locationRaw);

	if (fileObject != NULL)
	{
		*fileObject = std::move(fileTrack);
	}
	return location;
}

void iTunesOperations::libraryMap(std::function<void(trackPtr&)> processTrack, std::function<void(std::exception)> exceptionHandler)
{
	IITLibraryPlaylist* mainLibraryRaw;
	HandleCOMErrors(iTunes->get_LibraryPlaylist(&mainLibraryRaw));
	auto mainLibrary = WrapRawPtr(mainLibraryRaw);

	IITTrackCollection* libraryTracksRaw;
	HandleCOMErrors(mainLibrary->get_Tracks(&libraryTracksRaw));
	auto libraryTracks = WrapRawPtr(libraryTracksRaw);

	long libraryTracksCount;
	HandleCOMErrors(libraryTracks->get_Count(&libraryTracksCount));

	for (long trackIndex = 1; trackIndex <= libraryTracksCount; ++trackIndex)
	{
		try
		{
			IITTrack* trackRaw;
			HandleCOMErrors(libraryTracks->get_Item(trackIndex, &trackRaw));
			auto track = WrapRawPtr(trackRaw);

			processTrack(track);
		}
		catch (std::exception e)
		{
			exceptionHandler(e);
		}
	}

}

std::tstring iTunesOperations::GetFileName(const trackPtr & track)
{
	return GetFileName(track, NULL);
}

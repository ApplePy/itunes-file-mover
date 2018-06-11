#include "stdafx.h"
#include "ConversionHelpers.h"
#include "iTunesOperations.h"
#include <iostream>

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
	iTunes = std::unique_ptr <IiTunes, std::function<void(IiTunes*)>>(iTunesTmpRawPtr, SafeDeleteCOMObject<IiTunes>);
	iTunesTmpRawPtr = NULL;
}

void iTunesOperations::moveTrack(std::tstring sourcePrefix, std::tstring destinationPrefix, trackPtr& track)
{
	ITTrackKind trackType;
	HandleCOMErrors(track->get_Kind(&trackType));

	if (trackType != ITTrackKindFile) return;

	auto fileTrack = GetNewInterface<IITFileOrCDTrack>(static_cast<IUnknown*>(track.get()), IID_IITFileOrCDTrack);

	
}


iTunesOperations::~iTunesOperations()
{
	// Close up COM
	iTunes.reset();	// In case of reversed order (e.g. COM is closed before the object is released)
	CoUninitialize();
}


void iTunesOperations::libraryMap(std::function<void(trackPtr&)> processTrack)
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
		IITTrack* trackRaw;
		HandleCOMErrors(libraryTracks->get_Item(trackIndex, &trackRaw));
		auto track = WrapRawPtr(trackRaw);

		processTrack(track);
	}
}

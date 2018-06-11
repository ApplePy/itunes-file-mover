#include "stdafx.h"
#include "ConversionHelpers.h"
#include "iTunesOperations.h"


iTunesOperations::iTunesOperations()
{
	// Get COM started
	HRESULT comInitRes = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (comInitRes != S_OK && comInitRes != S_FALSE)
	{
		std::stringstream ss;
		ss << "Inititalization of COM library failed. Result was: " << comInitRes;
		throw std::runtime_error(ss.str().data());
	}

	// Get the iTunes library object
	IiTunes* iTunesTmpRawPtr;
	HandleCOMErrors(
		CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, reinterpret_cast<PVOID *>(&iTunesTmpRawPtr)),
		"Instantiation of iTunes COM interface failed. Result was: ");

	// Move the raw pointer into a smart pointer, and give it a safe-delete function.
	iTunes = std::unique_ptr <IiTunes, std::function<void(IiTunes*)>>(iTunesTmpRawPtr, SafeDeleteCOMObject<IiTunes>);
	iTunesTmpRawPtr = NULL;
}



iTunesOperations::~iTunesOperations()
{
	// Close up COM
	iTunes.reset();	// In case of reversed order (e.g. COM is closed before the object is released)
	CoUninitialize();
}

void iTunesOperations::processTrack(std::unique_ptr<IITTrack, std::function<void(IITTrack*)>>& track)
{
	ITTrackKind trackType;
	HandleCOMErrors(track->get_Kind(&trackType));
}


void iTunesOperations::moveLibrary(std::tstring sourcePrefix, std::tstring destinationPrefix)
{
	// TODO: I don't like this pattern. Could other processes use the track?

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

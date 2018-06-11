#pragma once
#include "stdafx.h"
#include "iTunes COM/iTunesCOMInterface.h"
#include "ConversionHelpers.h"

typedef std::unique_ptr<IITTrack, std::function<void(IITTrack*)>> trackPtr;

class iTunesOperations
{
public:
	iTunesOperations();
	void moveTrack(std::tstring sourcePrefix, std::tstring destinationPrefix, trackPtr& track);
	void libraryMap(std::function<void(trackPtr&)> processTrack);
	virtual ~iTunesOperations();

private:
	std::unique_ptr<IiTunes, std::function<void(IiTunes*)>> iTunes;
};


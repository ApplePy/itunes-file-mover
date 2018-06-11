#pragma once
#include "stdafx.h"
#include "iTunes COM/iTunesCOMInterface.h"
#include "ConversionHelpers.h"

class iTunesOperations
{
public:
	iTunesOperations();
	void moveLibrary(std::tstring sourcePrefix, std::tstring destinationPrefix);
	virtual ~iTunesOperations();

private:
	void processTrack(std::unique_ptr<IITTrack,std::function<void(IITTrack*)>>& track);
	std::unique_ptr<IiTunes, std::function<void(IiTunes*)>> iTunes;
};


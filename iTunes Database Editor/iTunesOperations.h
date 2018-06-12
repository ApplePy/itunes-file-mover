#pragma once
#include <memory>
#include <functional>
#include "THeaders.h"
#include "iTunes COM/iTunesCOMInterface.h"
#include "ConversionHelpers.h"

using trackPtr = ComPtr<IITTrack>;

class iTunesOperations
{
public:
	iTunesOperations();
	void moveTrack(std::tstring sourcePrefix, std::tstring destinationPrefix, trackPtr& track);
	void libraryMap(std::function<void(trackPtr&)> processTrack);
	virtual ~iTunesOperations();

private:
	ComPtr<IiTunes> iTunes;
};


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
	void moveTrack(std::tstring sourcePrefix, std::tstring destinationPrefix, const trackPtr& track);
	void libraryMap(std::function<void(trackPtr&)> processTrack, std::function<void(std::exception)> exceptionHandler = [](std::exception e) {throw e; });
	std::tstring GetFileName(const trackPtr& track);
	virtual ~iTunesOperations();

private:
	ComPtr<IiTunes> iTunes;

	std::tstring GetFileName(const trackPtr& track, ComPtr<IITFileOrCDTrack>* fileObject);
};


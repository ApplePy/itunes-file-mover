// iTunes File Mover.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iTunes COM/iTunesCOMInterface.h"
#include "iTunesOperations.h"


int main()
{
	iTunesOperations test;
	auto moveTrackOperation = std::bind(&iTunesOperations::moveTrack, &test, _T(R"(\\MURRAYMAIN\Shared Folders\Music\iTunes\Music)"), _T(R"(\\murraycan.london.murrayweb.ca\Shared Folders\Music\iTunes\Music)"), std::placeholders::_1);
	auto exceptionHandler = [](std::exception e) {std::cout << e.what() << std::endl; };
	test.libraryMap(moveTrackOperation, exceptionHandler);

	system("pause");

    return 0;
}



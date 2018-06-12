// iTunes File Mover.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iTunes COM/iTunesCOMInterface.h"
#include "iTunesOperations.h"


int main()
{
	iTunesOperations test;
	auto moveTrackOperation = std::bind(&iTunesOperations::moveTrack, &test, _T(R"(\\murraycan.london.murrayweb.ca\Shared Folders\Folder Redirection\Darryl\Music\iTunes\iTunes Media)"), _T("2"), std::placeholders::_1);
	auto exceptionHandler = [](std::exception e) {std::cout << e.what() << std::endl; };
	test.libraryMap(moveTrackOperation, exceptionHandler);

	system("pause");

    return 0;
}


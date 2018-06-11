// iTunes File Mover.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iTunes COM/iTunesCOMInterface.h"
#include "iTunesOperations.h"


int main()
{
	iTunesOperations test;
	auto moveTrackOperation = std::bind(&iTunesOperations::moveTrack, &test, _T("1"), _T("2"), std::placeholders::_1);
	test.libraryMap(moveTrackOperation);

	system("pause");

    return 0;
}


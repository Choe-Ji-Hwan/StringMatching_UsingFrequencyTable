#pragma once
#include "FrequencyTable.h"
#include <algorithm>

class RestoreMySequence {
public:
	RestoreMySequence(int cut, int originsize, std::string referFileName, std::string shortreadFileName);
	void doProcess(std::string resultFileName, int patternCutSize, int k);
	void showMatchingRate(std::string myFileName, std::string resultFileName);

private:

	std::string referline;
	std::vector<std::string> shortreadline;
	std::string resultline;
	std::string referencefilename;

	int cutSize;	// short read 자르는 크기 사이즈
	int originalSize;

	int patternCutSize;	// common 8, table 1열 사이즈
};
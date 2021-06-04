#pragma once
#include "MakeFileGenerator.h"
#include "FrequencyTable.h"
#include <algorithm>

class RestoreMySequence {
public:
	RestoreMySequence(int cut, int originsize, std::string referFileName, std::string shortreadFileName);
	void doProcess(std::string resultFileName, int patternCutSize);
	void showMatchingRate(std::string myFileName, std::string resultFileName);

private:
	//bool compare(std::pair<int, std::pair<int, int>>& a, std::pair<int, std::pair<int, int>>& b);

	std::string referline;
	std::string shortreadline;
	std::string resultline;

	int cutSize;
	int originalSize;
};
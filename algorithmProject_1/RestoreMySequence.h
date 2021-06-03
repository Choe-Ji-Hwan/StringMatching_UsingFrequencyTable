#pragma once
#include "MakeFileGenerator.h"
#include "FrequencyTable.h"
#include <algorithm>

class RestoreMySequence {
public:
	RestoreMySequence(int cut, int originsize, std::string referFileName, std::string shortreadFileName);
	void doProcess(std::string resultFileName, int patternCutSize);

private:
	bool compare(std::pair<int, int>& a, std::pair<int, int>& b);

	std::string referline;
	std::string shortreadline;
	std::string resultline;

	FrequencyTable* table;	// 테이블 초기 생성

	int cutSize;
	int originalSize;
};
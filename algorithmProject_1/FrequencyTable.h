#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <sstream>
#include "MakeFileGenerator.h"
class FrequencyTable {
public:

	FrequencyTable(std::string input, int cut, int stringSize);
	~FrequencyTable();
	void makeTable();

	std::string getPattern(int index);
	int getFrequency(int index);
	std::vector<int> getStartIndexArray(int index);

private:
	void makeRows();
	void fillTheTable(std::vector<char> combi, int index, int depth);
	void findStartIndex();

	char dnaString[4] = { 'A', 'C', 'G', 'T' };
	std::string originalString;
	int originalSize;
	int cutSize;

	int* frequency; // frequency column 따로
	std::string* pattern; // pattern column 따로
	std::vector<int>* startIndex; // start Index column 따로 

	int indexing; // 전체 index
};
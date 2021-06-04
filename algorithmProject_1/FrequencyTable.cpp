#include "FrequencyTable.h"

FrequencyTable::FrequencyTable(std::string input, int cut, int stringSize)
	: originalString(input), originalSize(stringSize), cutSize(cut), indexing(0) {

	int rowSize = (int)pow(4, cut);

	frequency = new int[rowSize]; // frequency table 따로
	pattern = new std::string[rowSize]; // pattern table 따로
	startIndex = new std::vector<int>[rowSize]; // start Index table 따로 

}

FrequencyTable::~FrequencyTable() {
	delete frequency;
	delete pattern;
	delete startIndex;
}

void FrequencyTable::makeTable() {
	std::cout << "table을 만들기 시작합니다." << std::endl;
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

	// 모든 경우의 수 넣기
	makeRows();	// 각각의 row들을 만드는 중.

	std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
	std::cout << "table 만드는데 걸리는 시간 " << sec.count() << " seconds" << std::endl;
}

std::string FrequencyTable::getPattern(int index) {
	return pattern[index];
}

int FrequencyTable::getFrequency(int index) {
	return frequency[index];
}

std::vector<int> FrequencyTable::getStartIndexArray(int index) {
	return startIndex[index];
}

void FrequencyTable::makeRows() // 열을 채우면서 행을 완성하면서 만들기
{
	std::vector<char> combi(originalSize);
	fillTheTable(combi, 0, 0);	// 첫번 째 열 채우기
	findStartIndex(); // 2, 3번째 열 채우기 
	std::cout << "열 완성" << std::endl;
}

void FrequencyTable::fillTheTable(std::vector<char> combi, int index, int depth) // 재귀를 이용해서 채우기 
{
	//dnaString
	if (depth == 8) {
		for (int i = 0; i < cutSize; i++) {
			pattern[indexing] += combi[i];	// 첫번째 열 채우기
		}
		indexing++;
		return;
	}
	else {
		for (int i = index; i < 4; i++) {
			combi[depth] = dnaString[i];
			fillTheTable(combi, index, depth + 1);
		}
	}
}


void FrequencyTable::findStartIndex() {

	MakeFileGenerator factory;

	std::string reference = factory.getFileString("reference.txt");
	std::istringstream ss(reference);

	std::string line, window;
	while (ss >> line) {	//\n을 구분자로 ShortRead를 문장단위로 가져옴
		for (int i = 0; i < originalSize - cutSize; i++) {
			int index = 0;
			window = line.substr(i, cutSize);
			for (int j = 0; j < 8; j++) {	//해당 빈도수 자리의 빈도값++
				switch (window[j])
				{
				case 'A':
					break;
				case 'C':
					index = index + pow(4, 7 - j);
					break;
				case 'G':
					index = index + 2 * pow(4, 7 - j);
					break;
				case 'T':
					index = index + 3 * pow(4, 7 - j);
					break;
				}
			}
			startIndex[index].push_back(i);
			frequency[index]++;
		}
	}
}
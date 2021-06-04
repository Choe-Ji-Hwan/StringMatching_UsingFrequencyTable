#include "FrequencyTable.h"

FrequencyTable::FrequencyTable(std::string input, int cut, int stringSize)
	: originalString(input), originalSize(stringSize), cutSize(cut), indexing(0) {

	int rowSize = (int)pow(4, cut);

	frequency = new int[rowSize]; // frequency table ����
	pattern = new std::string[rowSize]; // pattern table ����
	startIndex = new std::vector<int>[rowSize]; // start Index table ���� 

}

FrequencyTable::~FrequencyTable() {
	delete frequency;
	delete pattern;
	delete startIndex;
}

void FrequencyTable::makeTable() {
	std::cout << "table�� ����� �����մϴ�." << std::endl;
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

	// ��� ����� �� �ֱ�
	makeRows();	// ������ row���� ����� ��.

	std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
	std::cout << "table ����µ� �ɸ��� �ð� " << sec.count() << " seconds" << std::endl;
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

void FrequencyTable::makeRows() // ���� ä��鼭 ���� �ϼ��ϸ鼭 �����
{
	std::vector<char> combi(originalSize);
	fillTheTable(combi, 0, 0);	// ù�� ° �� ä���
	findStartIndex(); // 2, 3��° �� ä��� 
	std::cout << "�� �ϼ�" << std::endl;
}

void FrequencyTable::fillTheTable(std::vector<char> combi, int index, int depth) // ��͸� �̿��ؼ� ä��� 
{
	//dnaString
	if (depth == 8) {
		for (int i = 0; i < cutSize; i++) {
			pattern[indexing] += combi[i];	// ù��° �� ä���
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
	while (ss >> line) {	//\n�� �����ڷ� ShortRead�� ��������� ������
		for (int i = 0; i < originalSize - cutSize; i++) {
			int index = 0;
			window = line.substr(i, cutSize);
			for (int j = 0; j < 8; j++) {	//�ش� �󵵼� �ڸ��� �󵵰�++
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
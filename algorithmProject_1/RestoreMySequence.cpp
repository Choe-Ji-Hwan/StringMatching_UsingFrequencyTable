#include "RestoreMySequence.h"

RestoreMySequence::RestoreMySequence(int cut, int originsize, std::string referFileName, std::string shortreadFileName)
	: cutSize(cut), originalSize(originsize) {
	MakeFileGenerator factory;	// file���� string���� ��������
	referline = factory.getFileString(referFileName);
	shortreadline = factory.getFileString(shortreadFileName);
	resultline = "";
}

bool RestoreMySequence::compare(std::pair<int, int>& a, std::pair<int, int>& b) {
	if (table->getFrequency(a.second) == table->getFrequency(b.second))
		return table->getFrequency(a.second) > table->getFrequency(b.second);
	else return table->getFrequency(a.second) > table->getFrequency(b.second);
}

void RestoreMySequence::doProcess(std::string resultFileName, int patternCutSize) {

	MakeFileGenerator factory;
	factory.setFileRepeatString(originalSize, "result.txt", " ");
	std::string resultline = factory.getFileString("result.txt");
	table = new FrequencyTable(referline, patternCutSize, originalSize);
	table->makeTable();	// ���̺� �ϼ�


	int bufferSize = shortreadline.size() / cutSize;
	std::string* buffer = new std::string[bufferSize];
	int c = 0;
	for (int index = 0; index < bufferSize; index++) { // shortread�� ��� �и��ؼ� �迭�� ����
		buffer[index]=shortreadline.substr(c, cutSize);
		c += cutSize;
	}

	// �� buffer���� �� �� üũ
	for (int i = 0; i < bufferSize; i++) {

		std::pair<int, int> *minArray = new std::pair<int, int>[cutSize - patternCutSize];	// ��� ��
		int count = 0;
		std::string checkString = "";

		for (int j = 0; j < cutSize - patternCutSize; j++) {	// k=30, cut = 8, 30�� string���� 8�� �������� �ڸ���
			checkString = buffer[i].substr(j, patternCutSize);
			//std::cout << "checkstring: " << checkString << std::endl;
			int getindex = 0;
			// find string
			for (int k = 1; k <= patternCutSize; k++) {
				while (table->getPattern(getindex)[k - 1] != checkString[k - 1]) {
					getindex += (int)pow(4, patternCutSize - k);
				}
			}
			//std::cout << "real pattern: " << table.getPattern(getindex) << std::endl;
			std::pair<int, int> p1 = std::make_pair(j, getindex);
			minArray[j] = p1;	// �� ���
		}
		
		std::sort(minArray, minArray+ (cutSize-patternCutSize));	// frequency�� �������� �������� ����

		for (int ii = 0; ii < cutSize - patternCutSize; ii++) {
			std::cout << table->getPattern(minArray[ii].second) << "���� : " << table->getFrequency(minArray[ii].second) << std::endl;
		}

		int allowMismatches = 4;
		std::vector<int> mismatchesindex;
		bool isMatch = false;

		for (int in = 0; in < cutSize - patternCutSize; in++) {
			if (table->getFrequency(minArray[in].second) == 0) continue;

			int newGetIndex = minArray[in].second;
			int newindex = minArray[in].first;

			int mismatches = 0;
			// ���ؿ��� ���κ� üũ
			for (int o = 0; o < newindex; o++) {

				std::cout << "���� buffer[" << i << "][" << o << "] �� ��: " << buffer[i][o] << std::endl;
				std::cout << "�񱳴��: " << referline.at(newGetIndex - newindex + o) << std::endl;

				if (buffer[i][o] != referline.at(newGetIndex - newindex + o)) {
					mismatches++;	// �̽���ġ �߰�
					mismatchesindex.push_back(o);
					if (mismatches > allowMismatches) {
						break;
					}
				}
			}
			if (mismatches > allowMismatches) { // ���Ǵ� mismatch Ȯ��
				for (int p = 0; p < mismatchesindex.size(); p++)
					mismatchesindex.pop_back();
				continue;
			}
			// ������ �� ���� �ʿ����� �̹� ��ġ�ϴ�, pass

			// ���ؿ��� ���̺� üũ
			for (int p = newindex + patternCutSize; p < cutSize; p++) {
				std::cout << "���� buffer[" << i << "][" << p << "] �� ��: " << buffer[i][p] << std::endl;
				std::cout << "�񱳴��: " << referline.at(newGetIndex + patternCutSize + p) << std::endl;

				if (buffer[i][p] != referline.at(newGetIndex + patternCutSize + p)) {
					mismatches++;	// �̽���ġ �߰�
					mismatchesindex.push_back(p);
					if (mismatches > allowMismatches) {
						break;
					}
				}
			}
			if (mismatches > allowMismatches) { // ���Ǵ� mismatch Ȯ��
				for (int p = 0; p < mismatchesindex.size(); p++)
					mismatchesindex.pop_back();
				continue;
			}

			// ������� �Դٸ� ��ġ�ϴ� ����.
			if (mismatches <= allowMismatches) {	// mismatches �� ���ϸ�, k���� �� ������ ��.
				std::cout << "�ٲ� �� ���� " << std::endl;
				for (int o = 0; o < cutSize; o++) {
					resultline[newGetIndex - newindex + o] = buffer[i][o];	// �������� �ٲ��ֱ�
				}
				for (auto iter = mismatchesindex.begin(); iter != mismatchesindex.end(); iter++) {
					resultline[newGetIndex - newindex + *iter] = buffer[i][*iter]; // �ٲ��ֱ�
				}
				break;
			}
			for (int p = 0; p < mismatchesindex.size(); p++)
				mismatchesindex.pop_back();

		}
		delete minArray;
	}
	// ���Ͽ� �ֱ�
	factory.setFileString("result.txt", resultline);
	delete buffer;
	delete table;
}

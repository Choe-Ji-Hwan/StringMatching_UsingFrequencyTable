#include "RestoreMySequence.h"

RestoreMySequence::RestoreMySequence(int cut, int originsize, std::string referFileName, std::string shortreadFileName)
	: cutSize(cut), originalSize(originsize) {
	MakeFileGenerator factory;	// file���� string���� ��������
	referline = factory.getFileString(referFileName);
	shortreadline = factory.getStringForSR(shortreadFileName);
	resultline = "";
}

bool compare(std::pair<int, std::pair<int, int>>& a, std::pair<int, std::pair<int, int>>& b) {
	if (a.first == b.first)
		return a.first < b.first;
	else return a.first < b.first;
}

void RestoreMySequence::doProcess(std::string resultFileName, int patternCutSize) {
	std::cout << "restoring..." <<std::endl;
	MakeFileGenerator factory;
	factory.setFileRepeatString(originalSize, "result.txt", " ");
	std::string resultline = factory.getFileString("result.txt");

	//std::cout << "refer: " << referline.size() << std::endl;
	//std::cout << "short: " << shortreadline.size() << std::endl;

	std::cout << patternCutSize << "���� ��� ����� ���� ���� ������ table�� ����ϴ�." << std::endl;
	FrequencyTable* table = new FrequencyTable(referline, patternCutSize, originalSize);	// ���̺� �ʱ� ����t
	table->makeTable();	// ���̺� �ϼ�

	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	//std::cout << "���� �������: " << shortreadline.size()-1 << std::endl; 
	std::string* buffer = new std::string[shortreadline.size()-1]; 	// ������ "\n" ������ �ɷ���

	for (int i = 0; i < shortreadline.size()-1; i++) {
		buffer[i] = shortreadline[i];
	}

	// �� buffer���� �� �� üũ
	int i;
	for (i = 0; i < shortreadline.size()-1; i++) {	

		std::pair<int, std::pair<int, int>>* minArray = new std::pair<int, std::pair<int, int>>[cutSize - patternCutSize];	// ��� ��
		int count = 0;
		std::string checkString = "";
		int j;

		for (j = 0; j < cutSize - patternCutSize; j++) {	// k=30, cut = 8, 30�� string���� 8�� �������� �ڸ���
			checkString = buffer[i].substr(j, patternCutSize);
			//std::cout << "checkstring: " << checkString << std::endl;
			int getindex = 0;
			// find string
			for (int k = 1; k <= patternCutSize; k++) {
				while (table->getPattern(getindex)[k - 1] != checkString[k - 1]) {
					//std::cout << k-1 << std::endl;
					getindex += (int)pow(4, patternCutSize - k);
				}
			}
			//std::cout << "real pattern: " << table.getPattern(getindex) << std::endl;
			std::pair<int, int> p1 = std::make_pair(j, getindex);
			std::pair<int, std::pair<int, int>> p2 = std::make_pair(table->getFrequency(getindex), p1);
			minArray[j] = p2;	// �� ���
		}

		std::sort(&minArray[0], &minArray[(cutSize-patternCutSize)], compare);	// frequency�� �������� �������� ����

		//for (int ii = 0; ii < cutSize - patternCutSize; ii++) {
		//	std::cout << "fre: " << minArray[ii].first << ", ";
		//	std::cout << "index: " << minArray[ii].second.first << ", ";
		//	std::cout << "getindex: " << minArray[ii].second.second << std::endl;
		//}

		int allowMismatches = 4;
		std::vector<int> mismatchesindex;
		for (int in = 0; in < cutSize - patternCutSize; in++) {
			if (minArray[in].first == 0) continue;

			int newGetIndex = minArray[in].second.second;
			int newindex = minArray[in].second.first;

			//std::cout <<  "getindex: "<<newGetIndex << std::endl;
			//std::cout << "index: "<<newindex << std::endl;

			bool isfind = false;
			//std::cout << "\ngetindex: " << newGetIndex <<" ���õ� pattern�� " << table->getPattern(newGetIndex) << std::endl;
			for (int ref = 0; ref < table->getStartIndexArray(newGetIndex).size(); ref++) {

				if (table->getStartIndexArray(newGetIndex)[ref] - newindex < 0) continue;	// �̹� ���� ���Ͽ��� Ŀ����.
				std::string partReferLine = referline.substr(table->getStartIndexArray(newGetIndex)[ref]-newindex, cutSize);
				//std::cout << "������ �� " << partReferLine << std::endl;
				int mismatches = 0;
				// ���ؿ��� ���κ� üũ
				//std::cout << partReferLine << ": ���κ� üũ ��, ";
				for (int o = 0; o < newindex; o++) {

					//std::cout << "���� buffer[" << i << "][" << o << "] �� ��: " << buffer[i][o] << std::endl;
					//std::cout << "�񱳴��: " << partReferLine << std::endl;

					if (buffer[i][o] != partReferLine[o]) {
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
				//std::cout << "���̺� üũ �� " << std::endl;;
				for (int q = newindex + patternCutSize; q < cutSize; q++) {
					//std::cout << "���� buffer[" << i << "][" << q << "] �� ��: " << buffer[i][q] << std::endl;
					//std::cout << "�񱳴��: " << partReferLine << std::endl;

					if (buffer[i][q] != partReferLine[q]) {
						mismatches++;	// �̽���ġ �߰�
						mismatchesindex.push_back(q);
						if (mismatches > allowMismatches) {
							break;
						}
					}
				}
				if (mismatches > allowMismatches) { // ���Ǵ� mismatch Ȯ��

					for (int r = 0; r < mismatchesindex.size(); r++)
						mismatchesindex.pop_back();
					continue;
				}

				// ������� �Դٸ� ��ġ�ϴ� ����.
				//std::cout << "��ġȭ�� �� " << std::endl;
				if (mismatches <= allowMismatches) {	// mismatches �� ���ϸ�, k���� �� ������ ��.
					//std::cout << "�ٲ� �� ���� " << std::endl;
					int s;
					for (int s = 0; s < cutSize; s++) {
						resultline[table->getStartIndexArray(newGetIndex)[ref] - newindex + s] 
							= referline[table->getStartIndexArray(newGetIndex)[ref] - newindex + s];	// �������� �ٲ��ֱ�
					}
					for (auto iter = mismatchesindex.begin(); iter != mismatchesindex.end(); iter++) {
						resultline[table->getStartIndexArray(newGetIndex)[ref] - newindex + *iter] = buffer[i][*iter]; // �ش� mismatch�� �ٲ��ֱ�
					}
					isfind = true;
				}
				for (int t = 0; t < mismatchesindex.size(); t++)
					mismatchesindex.pop_back();
			}
			if (isfind) {
				isfind = false;
				break;
			}
		}
		//std::cout << "buffer count : " << i << std::endl;
		delete minArray;
	}
	// ���Ͽ� �ֱ�
	delete[] buffer;
	factory.setFileString("result.txt", resultline);
	std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
	std::cout << "��Ī�� �ɸ� �ð� : " << sec.count() << " seconds" << std::endl;
}

void RestoreMySequence::showMatchingRate(std::string myFileName, std::string resultFileName) {
	MakeFileGenerator factory;
	std::string my = factory.getFileString(myFileName);
	std::string re = factory.getFileString(resultFileName);

	int mismatch = 0;
	for (int i = 0; i < originalSize; i++) {
		if (my[i] != re[i]) mismatch++;
	}
	std::cout << "��ġ�� : " << (double)(originalSize - mismatch) / (originalSize) * 100 << "%" << std::endl;
}

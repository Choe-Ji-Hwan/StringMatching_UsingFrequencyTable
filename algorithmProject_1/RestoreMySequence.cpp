#include "RestoreMySequence.h"

RestoreMySequence::RestoreMySequence(int cut, int originsize, std::string referFileName, std::string shortreadFileName)
	: cutSize(cut), originalSize(originsize) {
	MakeFileGenerator factory;	// file에서 string으로 가져오기
	referline = factory.getFileString(referFileName);
	shortreadline = factory.getFileString(shortreadFileName);
	resultline = "";
}

bool compare(std::pair<int, std::pair<int, int>>& a, std::pair<int, std::pair<int, int>>& b) {
	if (a.first == b.first)
		return a.first < b.first;
	else return a.first < b.first;
}

void RestoreMySequence::doProcess(std::string resultFileName, int patternCutSize) {
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	MakeFileGenerator factory;
	factory.setFileRepeatString(originalSize, "result.txt", " ");
	std::string resultline = factory.getFileString("result.txt");

	//std::cout << "refer: " << referline.size() << std::endl;
	//std::cout << "short: " << shortreadline.size() << std::endl;

	FrequencyTable* table = new FrequencyTable(referline, patternCutSize, originalSize);	// 테이블 초기 생성t
	table->makeTable();	// 테이블 완성

	int bufferSize = shortreadline.size() / cutSize;
	std::string* buffer = new std::string[bufferSize];
	int c = 0;
	for (int index = 0; index < bufferSize; index++) { // shortread를 모두 분리해서 배열에 저장
		buffer[index]=shortreadline.substr(c, cutSize);
		c += cutSize;
	}

	// 각 buffer마다 빈도 수 체크
	int i;
	for (i = 0; i < bufferSize; i++) {

		std::pair<int, std::pair<int, int>>* minArray = new std::pair<int, std::pair<int, int>>[cutSize - patternCutSize];	// 기록 용
		int count = 0;
		std::string checkString = "";
		int j;
		for (j = 0; j < cutSize - patternCutSize; j++) {	// k=30, cut = 8, 30개 string에서 8개 기준으로 자르기
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
			std::pair<int, std::pair<int, int>> p2 = std::make_pair(table->getFrequency(getindex), p1);
			minArray[j] = p2;	// 빈도 기록
		}
		
		std::sort(&minArray[0], &minArray[(cutSize-patternCutSize)], compare);	// frequency를 기준으로 오름차순 정렬

		for (int ii = 0; ii < cutSize - patternCutSize; ii++) {
			std::cout << "fre: " << minArray[ii].first << ", ";
			std::cout << "index: " << minArray[ii].second.first << ", ";
			std::cout << "getindex: " << minArray[ii].second.second << std::endl;
		}

		int allowMismatches = 4;
		std::vector<int> mismatchesindex;

		for (int in = 0; in < cutSize - patternCutSize; in++) {
			if (minArray[in].first == 0) continue;

			int newGetIndex = minArray[in].second.second;
			int newindex = minArray[in].second.first;

			std::cout <<  "getindex: "<<newGetIndex << std::endl;
			std::cout << "index: "<<newindex << std::endl;

			bool isfind = false;

			for (int ref = 0; ref < table->getStartIndexArray(newGetIndex).size(); ref++) {
				std::string partReferLine = referline.substr(table->getStartIndexArray(newGetIndex)[ref]-newindex, cutSize);
				int mismatches = 0;
				// 기준에서 접두부 체크

				for (int o = 0; o < newindex; o++) {

					//std::cout << "현재 buffer[" << i << "][" << o << "] 의 값: " << buffer[i][o] << std::endl;
					//std::cout << "비교대상값: " << partReferLine << std::endl;

					if (buffer[i][o] != partReferLine[o]) {
						mismatches++;	// 미스매치 추가
						mismatchesindex.push_back(o);
						if (mismatches > allowMismatches) {
							break;
						}
					}
				}
				if (mismatches > allowMismatches) { // 허용되는 mismatch 확인

					for (int p = 0; p < mismatchesindex.size(); p++)
						mismatchesindex.pop_back();
					continue;
				}
				// 기준이 된 패턴 쪽에서는 이미 일치하니, pass

				// 기준에서 접미부 체크

				for (int q = newindex + patternCutSize; q < cutSize; q++) {
					//std::cout << "현재 buffer[" << i << "][" << q << "] 의 값: " << buffer[i][q] << std::endl;
					//std::cout << "비교대상값: " << partReferLine << std::endl;

					if (buffer[i][q] != partReferLine[q]) {
						mismatches++;	// 미스매치 추가
						mismatchesindex.push_back(q);
						if (mismatches > allowMismatches) {
							break;
						}
					}
				}
				if (mismatches > allowMismatches) { // 허용되는 mismatch 확인

					for (int r = 0; r < mismatchesindex.size(); r++)
						mismatchesindex.pop_back();
					continue;
				}

				// 여기까지 왔다면 일치하는 것임.
				if (mismatches <= allowMismatches) {	// mismatches 수 이하면, k까지 다 돌았을 것.
					//std::cout << "바뀐 것 존재 " << std::endl;
					int s;
					for (int s = 0; s < cutSize; s++) {
						resultline[table->getStartIndexArray(newGetIndex)[ref] - newindex + s] 
							= referline[table->getStartIndexArray(newGetIndex)[ref] - newindex + s];	// 본문에서 바꿔주기
					}
					for (auto iter = mismatchesindex.begin(); iter != mismatchesindex.end(); iter++) {
						resultline[table->getStartIndexArray(newGetIndex)[ref] - newindex + *iter] = buffer[i][*iter]; // 바꿔주기
					}
					isfind = true;
				}
				for (int t = 0; t < mismatchesindex.size(); t++)
					mismatchesindex.pop_back();
			}
			if (isfind) {
				break;
			}
		}
		std::cout << "buffer count : " << i << std::endl;
		delete minArray;
	}
	// 파일에 넣기
	delete[] buffer;
	factory.setFileString("result.txt", resultline);
	std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
	std::cout << "매칭에 걸린 시간 : " << sec.count() << " seconds" << std::endl;
}

void RestoreMySequence::showMatchingRate(std::string myFileName, std::string resultFileName) {
	MakeFileGenerator factory;
	std::string my = factory.getFileString(myFileName);
	std::string re = factory.getFileString(resultFileName);

	int mismatch = 0;
	for (int i = 0; i < originalSize; i++) {
		if (my[i] != re[i]) mismatch++;
	}
	std::cout << "일치율 : " << (double)(originalSize - mismatch) / (originalSize) * 100 << "%" << std::endl;
}

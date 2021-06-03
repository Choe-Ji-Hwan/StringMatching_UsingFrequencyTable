#include "RestoreMySequence.h"

RestoreMySequence::RestoreMySequence(int cut, int originsize, std::string referFileName, std::string shortreadFileName)
	: cutSize(cut), originalSize(originsize) {
	MakeFileGenerator factory;	// file에서 string으로 가져오기
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
	table->makeTable();	// 테이블 완성


	int bufferSize = shortreadline.size() / cutSize;
	std::string* buffer = new std::string[bufferSize];
	int c = 0;
	for (int index = 0; index < bufferSize; index++) { // shortread를 모두 분리해서 배열에 저장
		buffer[index]=shortreadline.substr(c, cutSize);
		c += cutSize;
	}

	// 각 buffer마다 빈도 수 체크
	for (int i = 0; i < bufferSize; i++) {

		std::pair<int, int> *minArray = new std::pair<int, int>[cutSize - patternCutSize];	// 기록 용
		int count = 0;
		std::string checkString = "";

		for (int j = 0; j < cutSize - patternCutSize; j++) {	// k=30, cut = 8, 30개 string에서 8개 기준으로 자르기
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
			minArray[j] = p1;	// 빈도 기록
		}
		
		std::sort(minArray, minArray+ (cutSize-patternCutSize));	// frequency를 기준으로 오름차순 정렬

		for (int ii = 0; ii < cutSize - patternCutSize; ii++) {
			std::cout << table->getPattern(minArray[ii].second) << "패턴 : " << table->getFrequency(minArray[ii].second) << std::endl;
		}

		int allowMismatches = 4;
		std::vector<int> mismatchesindex;
		bool isMatch = false;

		for (int in = 0; in < cutSize - patternCutSize; in++) {
			if (table->getFrequency(minArray[in].second) == 0) continue;

			int newGetIndex = minArray[in].second;
			int newindex = minArray[in].first;

			int mismatches = 0;
			// 기준에서 접두부 체크
			for (int o = 0; o < newindex; o++) {

				std::cout << "현재 buffer[" << i << "][" << o << "] 의 값: " << buffer[i][o] << std::endl;
				std::cout << "비교대상값: " << referline.at(newGetIndex - newindex + o) << std::endl;

				if (buffer[i][o] != referline.at(newGetIndex - newindex + o)) {
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
			for (int p = newindex + patternCutSize; p < cutSize; p++) {
				std::cout << "현재 buffer[" << i << "][" << p << "] 의 값: " << buffer[i][p] << std::endl;
				std::cout << "비교대상값: " << referline.at(newGetIndex + patternCutSize + p) << std::endl;

				if (buffer[i][p] != referline.at(newGetIndex + patternCutSize + p)) {
					mismatches++;	// 미스매치 추가
					mismatchesindex.push_back(p);
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

			// 여기까지 왔다면 일치하는 것임.
			if (mismatches <= allowMismatches) {	// mismatches 수 이하면, k까지 다 돌았을 것.
				std::cout << "바뀐 것 존재 " << std::endl;
				for (int o = 0; o < cutSize; o++) {
					resultline[newGetIndex - newindex + o] = buffer[i][o];	// 본문에서 바꿔주기
				}
				for (auto iter = mismatchesindex.begin(); iter != mismatchesindex.end(); iter++) {
					resultline[newGetIndex - newindex + *iter] = buffer[i][*iter]; // 바꿔주기
				}
				break;
			}
			for (int p = 0; p < mismatchesindex.size(); p++)
				mismatchesindex.pop_back();

		}
		delete minArray;
	}
	// 파일에 넣기
	factory.setFileString("result.txt", resultline);
	delete buffer;
	delete table;
}

#pragma once
#include "MakeFileGenerator.h"
#include "FrequencyTable.h"

class RestoreMySequence {
public:
	RestoreMySequence(int cut, int originsize, std::string referFileName, std::string shortreadFileName)
		: cutSize(cut), originalSize(originsize) {
		MakeFileGenerator factory;	// file에서 string으로 가져오기
		referline = factory.getFileString(referFileName);
		shortreadline = factory.getFileString(shortreadFileName);
	}

	void doProcess(std::string resultFileName, int patternCutSize) {
		std::string result = "";
		FrequencyTable table(referline, patternCutSize, originalSize);
		table.makeTable();	// 테이블 완성

		std::string* buffer = new std::string[shortreadline.size()/cutSize];
		for (int i = 0; i < originalSize;) { // 잘라서 보관
			shortreadline.substr(i, cutSize); 
			i += cutSize;
		}
		







		// 파일에 넣기
		MakeFileGenerator factory;
		factory.setFileString(resultFileName, result);

		delete buffer;
	}

private:
	std::string referline;
	std::string shortreadline;

	int cutSize;
	int originalSize;
};
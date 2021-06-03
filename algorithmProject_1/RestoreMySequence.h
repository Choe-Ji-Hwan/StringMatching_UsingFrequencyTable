#pragma once
#include "MakeFileGenerator.h"
#include "FrequencyTable.h"

class RestoreMySequence {
public:
	RestoreMySequence(int cut, int originsize, std::string referFileName, std::string shortreadFileName)
		: cutSize(cut), originalSize(originsize) {
		MakeFileGenerator factory;	// file���� string���� ��������
		referline = factory.getFileString(referFileName);
		shortreadline = factory.getFileString(shortreadFileName);
	}

	void doProcess(std::string resultFileName, int patternCutSize) {
		std::string result = "";
		FrequencyTable table(referline, patternCutSize, originalSize);
		table.makeTable();	// ���̺� �ϼ�

		std::string* buffer = new std::string[shortreadline.size()/cutSize];
		for (int i = 0; i < originalSize;) { // �߶� ����
			shortreadline.substr(i, cutSize); 
			i += cutSize;
		}
		







		// ���Ͽ� �ֱ�
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
#include <iostream>
#include "RestoreMySequence.h"
#include <chrono>
using namespace std;

#define REFER 1000000

// reference�� ����� (�Ϸ�)
// �� ���̺� �����. (�Ϸ�)

// short read ����� (�Ϸ�)
// short read�� �ٽ� �� �ڵ�
// short read�� ��Ī �˰���

#include <vector>
#include <math.h>

static std::string getFileString2(std::string file) {
	std::ifstream readfile;
	readfile.open(file);

	std::string fromfile;
	if (readfile.is_open()) {
		while (!readfile.eof()) {
			std::getline(readfile, fromfile);
		}
	}
	readfile.close();
	return fromfile;
}


int main() {
	MakeFileGenerator factory;

	// make table
	factory.makeRandomFile(REFER, "reference.txt");
	//string reference = factory.getFileString("reference.txt");

	// make mysequence, short read
	int k = 30;
	factory.makeMySqeunce(k, REFER, "reference.txt", "mysequence.txt");
	factory.makeShortRead(k, REFER, 40000, "mysequence.txt" , "shortread.txt");

	// matching
	int allpatternCut = 8;
	RestoreMySequence restore(k, REFER, "reference.txt", "shortread.txt");
	restore.doProcess("resultMySequence.txt", allpatternCut);
	restore.showMatchingRate("mysequence.txt", "result.txt");
}
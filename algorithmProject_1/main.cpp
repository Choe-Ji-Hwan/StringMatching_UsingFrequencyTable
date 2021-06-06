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
	MakeFileGenerator* factory = new MakeFileGenerator();

	// make table
	cout << REFER << "���� DNA reference ����" << endl;
	factory->makeRandomFile(REFER, "reference.txt");
	//string reference = factory.getFileString("reference.txt");

	// make mysequence, short read
	int k = 30;
	int n = 40000;
	factory->makeMySqeunce(k, REFER, "reference.txt", "mysequence.txt");
	cout << k << " �������� �������� " << n << "���� short read�� ����ϴ�." << endl;
	factory->makeShortRead(k, REFER, n, "mysequence.txt" , "shortread.txt");

	// matching
	int allpatternCut = 8;
	RestoreMySequence* restore = new RestoreMySequence(k, REFER, "reference.txt", "shortread.txt");
	restore->doProcess("resultMySequence.txt", allpatternCut);
	restore->showMatchingRate("mysequence.txt", "result.txt");

	delete factory;
	delete restore;
}
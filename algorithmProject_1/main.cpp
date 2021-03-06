#include <iostream>
#include "RestoreMySequence.h"
#include <chrono>
using namespace std;

#define REFER 1000000

// reference를 만들기 (완료)
// 빈도 테이블 만들기. (완료)

// short read 만들기 (완료)
// short read의 핵심 빈도 코드
// short read의 매칭 알고리즘

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
	//MakeFileGenerator* factory = new MakeFileGenerator();

	// make table
	//cout << REFER << "개의 DNA reference 생성" << endl;
	//factory->makeRandomFile(REFER, "ref.txt");
	//string reference = factory.getFileString("reference.txt");

	// make mysequence, short read

	int k = 100;
	int n = 11000;
	//factory->makeMySqeunce(k, REFER, "ref.txt", "mydna.txt");
	//cout << k << " 사이즈의 랜덤으로 " << n << "개의 short read를 만듭니다." << endl;
	//factory->makeShortRead(k, REFER, n, "mydna.txt" , "shortread.txt");

	// matching
	int allpatternCut = 8;
	RestoreMySequence* restore = new RestoreMySequence(k, REFER, "ref.txt", "shortread.txt");
	restore->doProcess("result.txt", allpatternCut, k);
	restore->showMatchingRate("mydna.txt", "result.txt");

	//delete factory;
	delete restore;
}
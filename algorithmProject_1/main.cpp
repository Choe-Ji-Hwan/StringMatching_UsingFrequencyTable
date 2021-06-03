#include <iostream>
#include "MakeFileGenerator.h"
#include "FrequencyTable.h"
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
	MakeFileGenerator factory;

	// make table
	factory.makeRandomFile(REFER, "reference.txt");
	string reference = factory.getFileString("reference.txt");
	//string reference = getFileString2("reference.txt");
	FrequencyTable table(reference, 8, REFER);
	table.makeTable();

	int k = 30;

	factory.makeMySqeunce(k, REFER, "reference.txt", "mysequence.txt");
	factory.makeShortRead(k, REFER, "mysequence.txt" , "shortread.txt");


}
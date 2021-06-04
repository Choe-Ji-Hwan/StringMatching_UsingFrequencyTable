#include "MakeFileGenerator.h"

// ATGC file size n

 void MakeFileGenerator::makeRandomFile(int n, std::string filename) {
	std::ofstream readfile;
	readfile.open(filename);
	readfile.clear();
	std::random_device rd;
	std::mt19937_64 mt(rd());
	std::uniform_int_distribution<int> dist(0, 3);

	for (int i = 0; i < n; i++) {
		switch (dist(mt))
		{
		case 0: readfile << 'A'; break;
		case 1: readfile << 'T'; break;
		case 2: readfile << 'G'; break;
		case 3: readfile << 'C'; break;
		default:
			break;
		}
	}
	readfile.close();
}

void MakeFileGenerator::makeMySqeunce(int k, int n, std::string originalFileName, std::string resultFileName) {
	 std::cout << "원본 my sqeunce 만들어지는 중" << std::endl;
	 std::string line;
	 std::ifstream readfile;
	 std::ofstream mySquence;
	 readfile.open(originalFileName);
	 mySquence.open(resultFileName);
	 readfile.clear();
	 mySquence.clear();

	 std::random_device rd;
	 std::mt19937_64 gen(rd());
	 std::uniform_int_distribution<int> distribute(0, k - 1);

	 std::vector<int> mismatches;

	 unsigned int i = 0;

	 if (readfile.is_open()) {
		 while (!readfile.eof()) {
			 getline(readfile, line);
			 for (i = 0; i < n - k; i++) {
				 if (i%k == 0) {	// k 길이마다
									// 랜덤으로 인덱스 값 2개 가져오기 (0~29)
					 int randomIndex1 = distribute(gen);
					 int randomIndex2 = distribute(gen);

					 for (unsigned int j = 0; j < k; j++) {
						 if (j == randomIndex1 || j == randomIndex2) {
							 // 여기서 둘다 변화가 없다면, 0개의 mismatch
							 // 한개만 변화 한다면, 1개의 mismatches
							 // 두개가 변화 한다면, 2개의 mismatches 가 만들어짐
							 switch (j % 4)
							 {
							 case 0: mySquence << 'A'; break;
							 case 1: mySquence << 'T'; break;
							 case 2: mySquence << 'G'; break;
							 case 3: mySquence << 'C'; break;
							 }
						 }
						 else {
							 mySquence << line.at(i + j);
						 }
					 }
					 i += (k - 1);
				 }
				 else {
					 mySquence << line.at(i);
				 }
			 }
		 }
		 // 나머지가 개수가 안맞는 경우 레퍼런스에서 그대로 복사
		 // 남는 것이 k개 이하이므로 그대로 복사.
		 if (n > i) {
			 int rest = line.size() - i;
			 std::string st = line.substr(i, rest);
			 mySquence << st;
		 }
	 }
	 readfile.close(); mySquence.close();
 }

void MakeFileGenerator::makeShortRead(int k, int n, int number, std::string originalFileName, std::string resultFileName) {
	std::cout << "short read가 만들어지는 중..." << std::endl;
	std::string line;
	std::ifstream mysequence;
	std::ofstream shortread;
	mysequence.open(originalFileName);
	shortread.open(resultFileName);
	mysequence.clear();
	shortread.clear();

	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<int> squencepick(0, n - 1 - k);

	std::vector<int> indexing(n);

	// indexing이 1인 pick을 shortread의 시작 index로
	int pick = squencepick(gen);
	for (int i = 0; i < number; i++) {
		while (indexing[pick] != 0) {
			pick = squencepick(gen);
		}
		indexing[pick]++;	// 랜덤으로 n개의 시작 주소 얻음
	}
	// 그것으로 short read를 만들어서 저장
	unsigned int i = 0;
	std::string shortreadstring;
	if (mysequence.is_open()) {
		while (!mysequence.eof()) {
			getline(mysequence, line);
			for (i = 0; i < n - k; i++) {

				if (indexing.at(i) == 0) continue;	// 1이면 그것을 시작 주소로 shortread로 읽음
				else {// indexing의 index 안에 값이 1이 경우
					shortreadstring += line.substr(i, k);
					shortreadstring += '\n';
				}
			}
		}
	}
	shortread << shortreadstring;
	shortread.close();  mysequence.close();
}

std::string MakeFileGenerator::getFileString(std::string filename) {
	std::ifstream readfile;
	readfile.open(filename);
	readfile.clear();
	std::string fromfile;
	if (readfile.is_open()) {
		while (!readfile.eof()) {
			std::getline(readfile, fromfile);
		}
	}
	readfile.close();
	return fromfile;
}

std::vector<std::string> MakeFileGenerator::getStringForSR(std::string filename) {
	std::ifstream readfile;
	readfile.open(filename);
	readfile.clear();
	std::string fromfile;

	std::vector<std::string> saveshortread;

	if (readfile.is_open()) {
		while (!readfile.eof()) {
			std::getline(readfile, fromfile);
			saveshortread.push_back(fromfile);
		}
	}
	readfile.close();
	return saveshortread;
}

void MakeFileGenerator::setFileString(std::string filename, std::string content) {
	std::ofstream writefile;
	writefile.open(filename);
	writefile.clear();
	if (writefile.is_open()) {
		writefile << content;
	}
	writefile.close();
}

void MakeFileGenerator::setFileRepeatString(int n, std::string filename, std::string content) {
	std::ofstream writefile;
	writefile.open(filename);
	writefile.clear();
	std::string temp = "";
	for (int i = 0; i < n; i++) {
		temp += content;
	}
	if (writefile.is_open()) {
		writefile << temp;
	}
	writefile.close();
}
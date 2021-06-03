#pragma once
#include <fstream>
#include <random>
#include <string>

class MakeFileGenerator {
public:

	// ATGC file size n
	void makeRandomFile(int n, std::string filename) {
		std::ofstream readfile;
		readfile.open(filename);

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

	void makeMySqeunce(int k, int n, std::string originalFileName, std::string resultFileName) {
		std::cout << "���� my sqeunce ��������� ��" << std::endl;
		std::string line;
		std::ifstream readfile;
		std::ofstream mySquence;
		readfile.open(originalFileName);
		mySquence.open(resultFileName);
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
					if (i%k == 0) {	// k ���̸���
						// �������� �ε��� �� 2�� �������� (0~29)
						int randomIndex1 = distribute(gen);
						int randomIndex2 = distribute(gen);

						for (unsigned int j = 0; j < k; j++) {
							if (j == randomIndex1 || j == randomIndex2) {
								// ���⼭ �Ѵ� ��ȭ�� ���ٸ�, 0���� mismatch
								// �Ѱ��� ��ȭ �Ѵٸ�, 1���� mismatches
								// �ΰ��� ��ȭ �Ѵٸ�, 2���� mismatches �� �������
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
			// �������� ������ �ȸ´� ��� ���۷������� �״�� ����
			// ���� ���� k�� �����̹Ƿ� �״�� ����.
			if (n > i) {
				int rest = line.size() - i;
				std::string st = line.substr(i, rest);
				mySquence << st;
			}
		}
		readfile.close(); mySquence.close();
	}

	void makeShortRead(int k, int n, std::string originalFileName, std::string resultFileName) {
		std::cout << "short read�� ��������� ��..." << std::endl;
		std::string line;
		std::ifstream mysequence;
		std::ofstream shortread;
		mysequence.open(originalFileName);
		shortread.open(resultFileName);
		shortread.clear();

		std::random_device rd;
		std::mt19937_64 gen(rd());
		std::uniform_int_distribution<int> squencepick(0, n - 1);

		std::vector<int> indexing(n);

		// indexing�� 1�� pick�� shortread�� ���� index��
		int pick = squencepick(gen);
		for (int i = 0; i < n; i++) {
			while (indexing[pick] != 0) {
				pick = squencepick(gen);
			}
			indexing[pick]++;	// �������� n���� ���� �ּ� ����
		}
		// �װ����� short read�� ���� ����
		unsigned int i = 0;
		std::string shortreadstring;
		if (mysequence.is_open()) {
			while (!mysequence.eof()) {
				getline(mysequence, line);
				for (i = 0; i < n - k; i++) {

					if (indexing.at(i) == 0) continue;	// 1�̸� �װ��� ���� �ּҷ� shortread�� ����
					else {// indexing�� index �ȿ� ���� 1�� ���
						shortreadstring += line.substr(i, k);
					}
				}
			}
		}
		shortread << shortreadstring;
		shortread.close();  mysequence.close();
	}

	std::string getFileString(std::string filename) {
		std::ifstream readfile;
		readfile.open(filename);

		std::string fromfile;
		if (readfile.is_open()) {
			while (!readfile.eof()) {
				std::getline(readfile, fromfile);
			}
		}
		readfile.close();
		return fromfile;
	}


private:
};
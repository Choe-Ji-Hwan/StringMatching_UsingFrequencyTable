#pragma once
#include <string>
#include <chrono>
#include <iostream>
class FrequencyTable {
public:
	enum DNA { A = 1, T = 2, G = 3, C = 4 };

	FrequencyTable(std::string input, int cut, int stringSize)
		: originalString(input), originalSize(stringSize), cutSize(cut), indexing(0) {

		int rowSize = (int)pow(4, cut);

		frequency = new int[rowSize]; // frequency table ����
		pattern = new std::string[rowSize]; // pattern table ����
		startIndex = new std::vector<int>[rowSize]; // start Index table ���� 

	}
	~FrequencyTable() {
		delete frequency;
		delete pattern;
		delete startIndex;
	}
	void makeTable() {
		std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
		std::string allCase = "";
		int count = 0;

		// ��� ����� �� �ֱ�
		makeRows();	// ������ row���� ����� ��.

		std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
		std::cout << "table ����µ� �ɸ��� �ð� " << sec.count() << " seconds" << std::endl;
	}

private:
	void makeRows() // ���� ä��鼭 ���� �ϼ��ϸ鼭 �����
	{
		std::vector<char> combi(originalSize);
		fillTheTable(combi, 0, 0);
	}
	void fillTheTable(std::vector<char> combi, int index, int depth) // ��͸� �̿��ؼ� ä��� 
	{
		//dnaString
		if (depth == 8) {
			for (int i = 0; i < cutSize; i++) {
				pattern[indexing] += combi[i];									// ù��° �� ä���
			}
			//std::cout << pattern[indexing] << ": ";
			int count = rabinKarp(pattern[indexing], cutSize, originalSize);	// ����° �� ä��� �� �ι� °�� �� ����
			frequency[indexing] = count;										// �ι�° �� ä���
			//std::cout << " [ " << count << " ] " << std::endl;
			std::cout << indexing << "�� �ϼ�" << std::endl;
			indexing++;
			return;
		}
		else {
			for (int i = index; i < 4; i++) {
				combi[depth] = dnaString[i];
				fillTheTable(combi, index, depth + 1);
			}
		}
	}
	// use Rabin Karp algorithm
	int rabinKarp(std::string pattern, int TOFIND, int NUM) {
		// ASCII 'A'=65, 'T'=84, 'G'=71, 'C'=67 % 5

		int cnt = 0;
		int h = 0;	// init
		int t = 0;	// init
		int d = 4;	// A,T,G,C
		int q = 13;
		int m = TOFIND;	// pattern length

		int D = fmod(pow(d, m - 1), q);		// D = d^(m-1) mod q
		for (int i = 0; i <= m - 1; i++) {
			h = fmod((d*h + pattern[i] % 5), q);	// ������ hash
			t = fmod((d*t + originalString[i] % 5), q);	// ������ string��ŭ �񱳵Ǵ� ���� hash
		}

		for (int s = 0; s < NUM - m + 1; s++) {
			if (h == t) {	// �ؽ� ���� ���� ���
				for (int i = 0; i < m; i++) {
					if (pattern[i] != originalString[s + i]) break;
					if (i + 1 == m) {
						cnt++;
						startIndex[indexing].push_back(s);
						//std::cout << ".";
					}
				}
			}
			if (s < NUM - m) {
				t = fmod((d*(t - originalString[s] % 5 * D) + originalString[s + m] % 5), q);	// ��ĭ �о �񱳵Ǵ� �� �ٽ� �ؽ� ���
				if (t < 0) t += q;	// ������ ���� ��� ���� ���� �ٽ� �� ����
			}
		}
		return cnt;
	}

private:
	char dnaString[4] = { 'A', 'C', 'G', 'T' };
	std::string originalString;
	int originalSize;
	int cutSize;

	int* frequency; // frequency column ����
	std::string* pattern; // pattern column ����
	std::vector<int>* startIndex; // start Index column ���� 

	int indexing; // ��ü index
};
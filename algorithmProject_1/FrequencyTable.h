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

		frequency = new int[rowSize]; // frequency table 따로
		pattern = new std::string[rowSize]; // pattern table 따로
		startIndex = new std::vector<int>[rowSize]; // start Index table 따로 

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

		// 모든 경우의 수 넣기
		makeRows();	// 각각의 row들을 만드는 중.

		std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
		std::cout << "table 만드는데 걸리는 시간 " << sec.count() << " seconds" << std::endl;
	}

private:
	void makeRows() // 열을 채우면서 행을 완성하면서 만들기
	{
		std::vector<char> combi(originalSize);
		fillTheTable(combi, 0, 0);
	}
	void fillTheTable(std::vector<char> combi, int index, int depth) // 재귀를 이용해서 채우기 
	{
		//dnaString
		if (depth == 8) {
			for (int i = 0; i < cutSize; i++) {
				pattern[indexing] += combi[i];									// 첫번째 열 채우기
			}
			//std::cout << pattern[indexing] << ": ";
			int count = rabinKarp(pattern[indexing], cutSize, originalSize);	// 세번째 열 채우기 및 두번 째열 값 리턴
			frequency[indexing] = count;										// 두번째 열 채우기
			//std::cout << " [ " << count << " ] " << std::endl;
			std::cout << indexing << "열 완성" << std::endl;
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
			h = fmod((d*h + pattern[i] % 5), q);	// 패턴의 hash
			t = fmod((d*t + originalString[i] % 5), q);	// 패턴의 string만큼 비교되는 것의 hash
		}

		for (int s = 0; s < NUM - m + 1; s++) {
			if (h == t) {	// 해시 값이 같을 경우
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
				t = fmod((d*(t - originalString[s] % 5 * D) + originalString[s + m] % 5), q);	// 한칸 밀어서 비교되는 것 다시 해시 계산
				if (t < 0) t += q;	// 음수가 나올 경우 나눈 수를 다시 더 해줌
			}
		}
		return cnt;
	}

private:
	char dnaString[4] = { 'A', 'C', 'G', 'T' };
	std::string originalString;
	int originalSize;
	int cutSize;

	int* frequency; // frequency column 따로
	std::string* pattern; // pattern column 따로
	std::vector<int>* startIndex; // start Index column 따로 

	int indexing; // 전체 index
};
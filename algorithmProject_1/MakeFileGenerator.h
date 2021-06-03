#pragma once
#include <fstream>
#include <iostream>
#include <random>
#include <string>

class MakeFileGenerator {
public:
	void makeRandomFile(int n, std::string filename);	// Ramdom ATGC file size n

	void makeMySqeunce(int k, int n, std::string originalFileName, std::string resultFileName);

	void makeShortRead(int k, int n, std::string originalFileName, std::string resultFileName);

	std::string getFileString(std::string filename);

	void setFileString(std::string filename, std::string content);
	void setFileRepeatString(int n, std::string filename, std::string content);
};
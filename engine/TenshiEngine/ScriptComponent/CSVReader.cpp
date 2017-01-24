#include "CSVReader.h"
#include <iostream>
#include <sstream>
#include <fstream>

CSVReader::CSVReader()
{
}

CSVReader::~CSVReader()
{
}

static std::vector<std::vector<std::string>> CSVReader::readCSV(std::string fileName){
	std::vector<std::vector<std::string>> arrays;
	//ファイルを開く
	std::ifstream ifs(fileName);
	if (!ifs) {
		Hx::Debug()->Log("CSVの名前が違う");
	}

	//CSVファイルを1行ずつ読み込む
	std::string str;
	std::vector<std::string> p;
	//1列読み込む
	while (std::getline(ifs,str)){
		std::string token;
		std::istringstream stream(str);
		//行に分ける
		while (std::getline(stream, token, ',')){
			//vectorに入れる
			p.push_back(token);
		}
		arrays.push_back(p);
	}
	return arrays;
}



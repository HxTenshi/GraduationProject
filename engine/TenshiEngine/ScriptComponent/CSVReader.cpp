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
	//�t�@�C�����J��
	std::ifstream ifs(fileName);
	if (!ifs) {
		Hx::Debug()->Log("CSV�̖��O���Ⴄ");
	}

	//CSV�t�@�C����1�s���ǂݍ���
	std::string str;
	std::vector<std::string> p;
	//1��ǂݍ���
	while (std::getline(ifs,str)){
		std::string token;
		std::istringstream stream(str);
		//�s�ɕ�����
		while (std::getline(stream, token, ',')){
			//vector�ɓ����
			p.push_back(token);
		}
		arrays.push_back(p);
	}
	return arrays;
}



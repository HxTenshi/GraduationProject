#include "FuckTenshiEngine.h"
#include "h_standard.h"
#include <istream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Mossan;

CSVScript::CSVScript()
{
}

CSVScript::~CSVScript()
{
}

void CSVScript::writeCSV(std::string fileName, std::vector<std::vector<std::string>> arrays){
	//�t�@�C�����J��
	ofstream ofs(fileName);
	if (!ofs) {
		Hx::Debug()->Log("CSV�̖��O���Ⴄ");
	}

	for (int i = 0; i < arrays.size(); i++) {
		for (int j = 0; j < arrays[i].size(); j++) {
			//��������
			ofs << arrays[i][j] << "," << endl;
		}
	}
}

std::vector<std::vector<std::string>> CSVScript::readCSV(std::string fileName)
{
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
	while (std::getline(ifs, str)) {
		std::string token;
		std::istringstream stream(str);
		//�s�ɕ�����
		while (std::getline(stream, token, ',')) {
			//vector�ɓ����
			p.push_back(token);
		}
		arrays.push_back(p);
	}
	return arrays;
}
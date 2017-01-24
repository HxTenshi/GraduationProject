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
	//ファイルを開く
	ofstream ofs(fileName);
	if (!ofs) {
		Hx::Debug()->Log("CSVの名前が違う");
	}

	for (int i = 0; i < arrays.size(); i++) {
		for (int j = 0; j < arrays[i].size(); j++) {
			//書き込み
			ofs << arrays[i][j] << "," << endl;
		}
	}
}

std::vector<std::vector<std::string>> CSVScript::readCSV(std::string fileName)
{
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
	while (std::getline(ifs, str)) {
		std::string token;
		std::istringstream stream(str);
		//行に分ける
		while (std::getline(stream, token, ',')) {
			//vectorに入れる
			p.push_back(token);
		}
		arrays.push_back(p);
	}
	return arrays;
}
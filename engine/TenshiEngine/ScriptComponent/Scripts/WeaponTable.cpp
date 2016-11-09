#include "WeaponTable.h"
#include<fstream>
#include<iostream>
#include<sstream>
#include <vector>
//生成時に呼ばれます（エディター中も呼ばれます）
void WeaponTable::Initialize() {
	int a;
	int b;
	float c;
	std::string d = "guriruchang";
	//LoadWeaponParametor("Assets/data/type.csv");
	//LoadCommon<int,int,float,std::string>("Assets/data/type.csv", a, b, c, d, 
	//[&](int a,int b,float c,std::string d) {
	//	Hx::Debug()->Log(d);
	//});
	std::vector<int> testparam;
	Load("Assets/data/type.csv");
	testparam = GetArrayParams<int>(3);
	for (auto i : testparam) {
		std::stringstream ss;
		ss << i;
		Hx::Debug()->Log(ss.str());
	}
	//DebugLog();
}
//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WeaponTable::Start(){
	
}

//毎フレーム呼ばれます
void WeaponTable::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void WeaponTable::Finish(){

}

//コライダーとのヒット時に呼ばれます
void WeaponTable::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void WeaponTable::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void WeaponTable::OnCollideExit(GameObject target){
	(void)target;
}

void WeaponTable::TableInit()
{
}

void WeaponTable::AddMagnification(std::string name, funifuni::WeaponMagnification mag)
{
	m_Magnification.insert(std::make_pair(name, mag));
}

void WeaponTable::AddParametor(std::string name, funifuni::WeaponParametor param)
{
	m_Param.insert(std::make_pair(name, param));
}

void WeaponTable::LoadMagnification(std::string magname)
{
	std::ifstream ifs(magname);
	if (!ifs) {
		return;
	}

	//csvファイルを1行ずつ読み込む
	std::string str;
	std::vector<std::string> p;
	while (getline(ifs, str)) {
		std::string token;
		std::istringstream stream(str);

		while (getline(stream, token, ',')) {
			p.push_back(token);
		}
		funifuni::WeaponMagnification mg;
		int count = 0;
		for (auto i = p.begin(); i != p.end(); ++i) {
			if (count != 0) {
				mg.AddMag(std::stof(i->data()));
			}
			++count;
		}
		AddMagnification(p.data()[0], mg);
		p.clear();
	}
}

void WeaponTable::LoadWeaponParametor(std::string paramname)
{
	std::ifstream ifs(paramname);
	if (!ifs) {
		return;
	}

	//csvファイルを1行ずつ読み込む
	std::string str;
	std::vector<std::string> p;
	while (getline(ifs, str)) {
		std::string token;
		std::istringstream stream(str);

		while (getline(stream, token, ',')) {
			p.push_back(token);
		}
		funifuni::WeaponParametor wp;
		auto data = p.data();
		wp.SetName(data[1]);
		wp.SetDurable(std::stoi(data[2]));
		wp.SetAttack(std::stof(data[3]));
		AddParametor(data[1], wp);
		std::cout << std::endl;
		p.clear();
	}
}


void WeaponTable::DebugLog()
{

	for (auto i = m_Param.begin(); i != m_Param.end(); ++i) {
		std::ostringstream attack;
		attack << i->second.AttackParam();
		std::ostringstream durable;
		durable << i->second.GetDurable();
		Hx::Debug()->Log("\n名前:" + i->first + "\n" +
			"攻撃力:" + attack.str() + "\n" +
			"耐久値:" + durable.str());
	}
}

void WeaponTable::Load(std::string fname)
{
	std::ifstream ifs(fname);
	if (!ifs) {
		return;
	}

	//csvファイルを1行ずつ読み込む
	std::string str;
	int count = 0;
	while (getline(ifs, str)) {
		std::string token;
		std::istringstream stream(str);
		std::vector<std::string> buff;
		count = 0;
		while (getline(stream, token, ',')) {
			buff.push_back(token);
			count++;
		}
		m_data.push_back(buff);
	}
	m_maxcount = count;
}


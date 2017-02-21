#include "WeaponTable.h"
#include<fstream>
#include<iostream>
#include<sstream>
#include <vector>

static WeaponTable* g_WeaponTable = NULL;
WeaponTable::WeaponTable()
{



	std::vector<std::string> type;
	std::vector<std::string> name;
	std::vector<int> durable;
	std::vector<int> atk;
	Load("Assets/data/type.csv");
	type = GetArrayParams<std::string>(0);
	name = GetArrayParams<std::string>(1);
	durable = GetArrayParams<int>(2);
	atk = GetArrayParams<int>(3);

	int weaponcount = name.size();
	int count = 0;
	for (auto i : name) {
		funifuni::WeaponParametor p;
		p.SetWeaponType(type.data()[count]);
		p.SetName(name.data()[count]);
		p.SetDurable(durable.data()[count]);
		p.SetAttack(atk.data()[count]);
		p.SetDurableDamage(2.0f, 3.0f);
		p.SetWeaponType(i);
		m_id.insert(std::pair<int, std::string>(count, i));
		m_Param.insert(std::pair < std::string, funifuni::WeaponParametor>(i, p));
		count++;
	}

	//DebugLog();
}
//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponTable::Initialize() {
	//LoadWeaponParametor("Assets/data/type.csv");
	//LoadCommon<int,int,float,std::string>("Assets/data/type.csv", a, b, c, d, 
	//[&](int a,int b,float c,std::string d) {
	//	Hx::Debug()->Log(d);
	//});
	g_WeaponTable = this;

}
//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponTable::Start(){
	
}

//���t���[���Ă΂�܂�
void WeaponTable::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void WeaponTable::Finish(){
	g_WeaponTable = NULL;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WeaponTable::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WeaponTable::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void WeaponTable::OnCollideExit(GameObject target){
	(void)target;
}

void WeaponTable::TableInit()
{
}
void WeaponTable::LoadWeaponParametor(std::string paramname)
{
	std::ifstream ifs(paramname);
	if (!ifs) {
		return;
	}

	//csv�t�@�C����1�s���ǂݍ���
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
		std::cout << std::endl;
		p.clear();
	}
}


void WeaponTable::DebugLog()
{
	return;
	for (auto i = m_Param.begin(); i != m_Param.end(); ++i) {
		std::ostringstream attack;
		attack << i->second.AttackParam();
		std::ostringstream durable;
		durable << i->second.GetDurable();
		Hx::Debug()->Log("\n���O:" + i->first + "\n" +
			"�U����:" + attack.str() + "\n" +
			"�ϋv�l:" + durable.str());
	}
	//Hx::Debug()->Log(std::to_string(m_Param["���C�X"].AttackParam()));

}

funifuni::WeaponParametor& WeaponTable::GetWeaponParametor(std::string name)
{

	//Hx::Debug()->Log(std::to_string(m_Param[name].AttackParam()));
	return m_Param[name];
}

void WeaponTable::Load(std::string fname)
{
	std::ifstream ifs(fname);
	if (!ifs) {
		return;
	}

	//csv�t�@�C����1�s���ǂݍ���
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

std::string WeaponTable::IDtoName(int id)
{
	if (0 > id | m_maxcount < id - 1)return "���m�̌�";
	return m_id[id];
}

WeaponTable * WeaponTable::GetWeaponTable()
{
	return g_WeaponTable;
}


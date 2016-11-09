#include "WeaponCommon.h"


namespace funifuni {

	//*****************************************************//
	//					WeaponMagnification
	//*****************************************************//
	//�R���X�g���N�^
	WeaponMagnification::WeaponMagnification()
	{
	}
	//�{���̒ǉ�
	void WeaponMagnification::AddMag(float mag)
	{
		m_Magnification.push_back(mag);
	}
	//�{���̎擾
	float WeaponMagnification::GetMagnification(WeaponMagnificationType type)
	{
		if (m_Magnification.size() < (int)type)return 0.0f;
		return m_Magnification[(int)type];
	}

	int WeaponMagnification::GetLange()
	{
		return m_Magnification.size();
	}
	//*****************************************************//
	//					  WeaponParametor
	//*****************************************************//
	/// <summary>
	///�@�R���X�g���N�^
	/// </summary>
	WeaponParametor::WeaponParametor()
	{
	}
	/// <summary>
	///�@�f�X�g���N�^
	/// </summary>
	WeaponParametor::~WeaponParametor()
	{
	}
	/// <summary>
	///�{���̃e�[�u������{���̎擾�B
	/// </summary>
	const WeaponMagnification& WeaponParametor::GetMagnification()
	{
		return m_Magnification;
	}
	/// <summary>
	///���O�̃Z�b�g
	/// </summary>
	void WeaponParametor::SetName(std::string name)
	{
		m_Name = name;
	}
	/// <summary>
	///�U���͂̃Z�b�g
	/// </summary>
	void WeaponParametor::SetAttack(float attack)
	{
		m_AttackParam = attack;
	}
	/// <summary>
	///�{���̃e�[�u���̎Q�Ƃ�ϐ��Ɋi�[
	/// </summary>
	void WeaponParametor::SetMagnification(const WeaponMagnification & mag)
	{
		m_Magnification = mag;
	}
	/// <summary>
	///�ϋv�l�̃Z�b�g
	/// </summary>
	void WeaponParametor::SetDurable(int durable)
	{
		m_Durable = durable;
	}
	/// <summary>
	///�ϋv�l�̃_���[�W�̐ݒ�
	/// </summary>
	void WeaponParametor::SetDurableDamage(int low, int strong)
	{
		m_LowDurableDamage = low;
		m_StrongDurableDamage = strong;
	}
	void WeaponParametor::SetAllParam(std::string name, float attack, int durable, int low, int strong)
	{
		SetName(name);
		SetAttack(attack);
		SetDurable(durable);
		SetDurableDamage(low, strong);
	}
	/// <summary>
	///�ϋv�l�ւ̃_���[�W
	/// </summary>
	void WeaponParametor::DurableDamage(WeaponMagnificationType type)
	{
		if (isOverLange(type))return;
		if (type == WeaponMagnificationType::Mag_Deathblow) {
			m_Durable = 0;
			return;
		}
		if (type <= WeaponMagnificationType::Mag_Fly_Weak) {
			m_Durable -= m_LowDurableDamage;

		}
		else {
			m_Durable -= m_StrongDurableDamage;
		}
	}
	/// <summary>
	///����̑ϋv�l���Ȃ��Ȃ�����
	/// </summary>
	bool WeaponParametor::isBreak()
	{
		return (m_Durable <= 0) ? true : false;
	}
	/// <summary>
	///�ϋv�l�̎擾
	/// </summary>
	int WeaponParametor::GetDurable()
	{
		return m_Durable;
	}
	/// <summary>
	///�U���͂̎擾
	/// </summary>
	float WeaponParametor::AttackParam()
	{
		return m_AttackParam;
	}
	/// <summary>
	///�U�����̃_���[�W�̎擾
	/// </summary>
	float WeaponParametor::AttackDamage(WeaponMagnificationType type)
	{
		if (isOverLange(type))return 0.0f;

		float mag = m_Magnification.GetMagnification(type);

		return m_AttackParam*mag;
	}
	/// <summary>
	///�ϋv�l�̃e�[�u������擾�ł��邩�H
	/// </summary>
	bool WeaponParametor::isOverLange(WeaponMagnificationType type)
	{
		return m_Magnification.GetLange() > (int)type;
	}

	//CSVLoader::CSVLoader()
	//{
	//}

	//void CSVLoader::Load(std::string fname)
	//{
	//	std::ifstream ifs(fname);
	//	if (!ifs) {
	//		return;
	//	}

	//	//csv�t�@�C����1�s���ǂݍ���
	//	std::string str;
	//	while (getline(ifs, str)) {
	//		std::string token;
	//		std::istringstream stream(str);

	//		while (getline(stream, token, ',')) {
	//			int temp = std::stof(token);
	//			std::cout << temp << ",";
	//		}
	//		std::cout << std::endl;
	//	}
	//}

	//void CSVLoader::Load(std::string fname, const std::function<void(std::string, float, int, int, int)>& func)
	//{

	//	std::ifstream ifs(fname);
	//	if (!ifs) {
	//		return;
	//	}

	//	//csv�t�@�C����1�s���ǂݍ���
	//	std::string str;
	//	std::vector<std::string> p;
	//	while (getline(ifs, str)) {
	//		std::string token;
	//		std::istringstream stream(str);

	//		while (getline(stream, token, ',')) {
	//			//float temp = stof(token);
	//			p.push_back(token);
	//		}
	//		func(p.data()[0], std::stof(p.data()[1]), std::stoi(p.data()[2]), std::stoi(p.data()[3]), std::stoi(p.data()[4]));
	//		std::cout << std::endl;
	//		p.clear();
	//	}
	//}

	//void CSVLoader::LoadWeapon(std::string paramname, WeaponTable* table)
	//{
	//	std::ifstream ifs(paramname);
	//	if (!ifs) {
	//		return;
	//	}

	//	//csv�t�@�C����1�s���ǂݍ���
	//	std::string str;
	//	std::vector<std::string> p;
	//	while (getline(ifs, str)) {
	//		std::string token;
	//		std::istringstream stream(str);

	//		while (getline(stream, token, ',')) {
	//			p.push_back(token);
	//		}
	//		WeaponParametor wp;
	//		auto data = p.data();
	//		wp.SetName(data[1]);
	//		wp.SetDurable(std::stoi(data[2]));
	//		wp.SetAttack(std::stof(data[3]));
	//		table->AddParametor(data[1], wp);
	//		std::cout << std::endl;
	//		p.clear();
	//	}
	//}

	//void CSVLoader::LoadMagnification(std::string magname, WeaponTable* table)
	//{
	//	std::ifstream ifs(magname);
	//	if (!ifs) {
	//		return;
	//	}

	//	//csv�t�@�C����1�s���ǂݍ���
	//	std::string str;
	//	std::vector<std::string> p;
	//	while (getline(ifs, str)) {
	//		std::string token;
	//		std::istringstream stream(str);
	//		
	//		while (getline(stream, token, ',')) {
	//			p.push_back(token);
	//		}
	//		WeaponMagnification mg;
	//		int count = 0;
	//		for (auto i = p.begin(); i != p.end(); ++i) {
	//			if (count != 0) {
	//				mg.AddMag(std::stof(i->data()));
	//			}
	//			++count;
	//		}
	//		table->AddMagnification(p.data()[0], mg);
	//		p.clear();
	//	}
	//}
}

#include "WeaponCommon.h"
#include <sstream>

namespace funifuni {
	//*****************************************************//
	//					  WeaponParametor
	//*****************************************************//
	/// <summary>
	///　コンストラクタ
	/// </summary>
	WeaponParametor::WeaponParametor()
	{
	}
	/// <summary>
	///　デストラクタ
	/// </summary>
	WeaponParametor::~WeaponParametor()
	{
	}

	/// <summary>
	///名前のセット
	/// </summary>
	void WeaponParametor::SetName(std::string name)
	{
		m_Name = name;
	}
	/// <summary>
	///攻撃力のセット
	/// </summary>
	void WeaponParametor::SetAttack(float attack)
	{
		m_AttackParam = attack;
	}
	/// <summary>
	///耐久値のセット
	/// </summary>
	void WeaponParametor::SetDurable(float durable)
	{
		m_Durable = durable;
		m_MaxDurable = durable;
	}
	void WeaponParametor::SetChangeMaxDurable(float durable)
	{
		m_Durable = durable;
	}
	/// <summary>
	///耐久値のダメージの設定
	/// </summary>
	void WeaponParametor::SetDurableDamage(float low, float strong)
	{
		m_LowDurableDamage = low;
		m_StrongDurableDamage = strong;
	}
	void WeaponParametor::SetWeaponType(WeaponType type)
	{
		m_Type = type;
	}
	void WeaponParametor::SetWeaponType(std::string type)
	{
		if (type == "Sword")m_Type = WeaponType::Sword;
		if (type == "Rance")m_Type = WeaponType::Rance;
		if (type == "Axe")m_Type = WeaponType::Axe;
		if (type == "SwordDH")m_Type = WeaponType::SwordDH;
		if (type == "Other")m_Type = WeaponType::Other;
	}
	WeaponType WeaponParametor::GetWeaponType()
	{
		return m_Type;
	}
	void WeaponParametor::Damage(DamageType type,float mag = 1.0f)
	{
		if(type==DamageType::LowDamage)m_Durable -= m_LowDurableDamage*mag;
		if (type == DamageType::HighDamage)m_Durable -= m_StrongDurableDamage*mag;
		if (type == DamageType::DethBrowDamage)m_Durable = 0;
		if (type == DamageType::BreakDamage)m_Durable = mag;
	}
	float WeaponParametor::GetMaxDurable()
	{
		return m_MaxDurable;
	}

	void WeaponParametor::DebugLog()
	{
		return;
		std::ostringstream atk;
		atk << m_AttackParam;
		std::ostringstream durable;
		durable << m_Durable;
		Hx::Debug()->Log("\n名前："+m_Name+"\n"+
			"攻撃力:"+atk.str()+"\n"+
			"耐久値:"+durable.str());
	}

	/// <summary>
	///武器の耐久値がなくなったか
	/// </summary>
	bool WeaponParametor::isBreak()
	{
		return (m_Durable <= 0) ? true : false;
	}
	/// <summary>
	///耐久値の取得
	/// </summary>
	float WeaponParametor::GetDurable()
	{
		return m_Durable;
	}
	/// <summary>
	///攻撃力の取得
	/// </summary>
	float WeaponParametor::AttackParam()
	{
		return m_AttackParam;
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

	//	//csvファイルを1行ずつ読み込む
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

	//	//csvファイルを1行ずつ読み込む
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

	//	//csvファイルを1行ずつ読み込む
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

	void Tween::SetTween(float & v, float end, float d)
	{
		value = v;
		start = v;
		end = end;
		duration = d;
		frame = 0.0f;
	}

	bool Tween::Update()
	{
		frame += step;
		float t = (frame / duration);
		value = start*(1.0f - t) + end*t;
		return frame <= duration;
	}

	//	//csvファイルを1行ずつ読み込む
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

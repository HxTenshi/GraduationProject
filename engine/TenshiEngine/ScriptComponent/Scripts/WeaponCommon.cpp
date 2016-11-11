#include "WeaponCommon.h"


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
	void WeaponParametor::SetDurable(int durable)
	{
		m_Durable = durable;
	}
	/// <summary>
	///耐久値のダメージの設定
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
	///武器の耐久値がなくなったか
	/// </summary>
	bool WeaponParametor::isBreak()
	{
		return (m_Durable <= 0) ? true : false;
	}
	/// <summary>
	///耐久値の取得
	/// </summary>
	int WeaponParametor::GetDurable()
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

#include "WeaponCommon.h"


namespace funifuni {

	//*****************************************************//
	//					WeaponMagnification
	//*****************************************************//
	//コンストラクタ
	WeaponMagnification::WeaponMagnification()
	{
	}
	//倍率の追加
	void WeaponMagnification::AddMag(float mag)
	{
		m_Magnification.push_back(mag);
	}
	//倍率の取得
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
	///倍率のテーブルから倍率の取得。
	/// </summary>
	const WeaponMagnification& WeaponParametor::GetMagnification()
	{
		return m_Magnification;
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
	///倍率のテーブルの参照を変数に格納
	/// </summary>
	void WeaponParametor::SetMagnification(const WeaponMagnification & mag)
	{
		m_Magnification = mag;
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
	///耐久値へのダメージ
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
	/// <summary>
	///攻撃時のダメージの取得
	/// </summary>
	float WeaponParametor::AttackDamage(WeaponMagnificationType type)
	{
		if (isOverLange(type))return 0.0f;

		float mag = m_Magnification.GetMagnification(type);

		return m_AttackParam*mag;
	}
	/// <summary>
	///耐久値のテーブルから取得できるか？
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


#pragma once
#include "main.h"
#include <map>
#include <string>
#include "../h_standard.h"

#include "WeaponCommon.h"

class WeaponTable :public IDllScriptComponent{
public:
	static WeaponTable* get() {
		static WeaponTable w;
		return &w;
	}
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void TableInit();
	void LoadWeaponParametor(std::string paramname);
	void DebugLog();
	template<class... Args>
	void LoadCommon(std::string fname, Args&... arg,std::function<void(Args&... arg)> func);

	void Load(std::string fname);
	template<typename T>
	std::vector<T> GetArrayParams(int array_num) {
		std::vector<T> buff;
		for (auto i : m_data) {
			buff.push_back(LoaderValueType<T>::convert(i.data()[array_num]));
		}
		return buff;
	}
	template<class check, class out>
	out convert(out param) {
		return LoaderValueType<check>::isType(param);
	}

private:
	//攻撃力
	std::map<std::string, funifuni::WeaponParametor> m_Param;
	//ｃｓｖ
	std::vector<std::vector<std::string>> m_data;
	int m_maxcount;
};
template<typename T>
struct LoaderValueType {

	static T convert(std::string data) {
		return NULL;
	}
};
template<>
struct LoaderValueType<int> {
	static int convert(std::string data) {
		return std::stoi(data);
	}
};
template<>
struct LoaderValueType<float> {
	static float convert(std::string data) {
		return std::stof(data);
	}
};
template<>
struct LoaderValueType<double> {
	static double convert(std::string data) {
		return std::stod(data);
	}
};
template<>
struct LoaderValueType<long double> {
	static double convert(std::string data) {
		return std::stold(data);
	}
};
template<>
struct LoaderValueType<bool> {
	static bool convert(std::string data) {
		return (bool)std::stoi(data);
	}
};
template<class... Args>
inline void WeaponTable::LoadCommon(std::string fname, Args & ...arg, std::function<void(Args& ...arg)> func)
{
	func(arg...);
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

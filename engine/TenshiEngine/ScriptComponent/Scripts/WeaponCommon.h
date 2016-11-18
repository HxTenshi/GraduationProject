
#pragma once
#include "main.h"
#include <vector>
#include <functional>
#include<string>


enum DamageType {
	LowDamage,
	HighDamage,
	DethBrowDamage
};
enum WeaponType {
	Sword,
	Rance,
	Axe
};

class WeaponCommon :public IDllScriptComponent{
	
};

namespace funifuni {

	//*****************************************************//
	//					WeaponParametor
	//*****************************************************//
	class WeaponParametor {
	public:
		WeaponParametor();
		~WeaponParametor();


		//耐久値が無くなったか
		bool isBreak();
		//耐久値の取得
		float GetDurable();

		//攻撃力の取得
		float AttackParam();

		//****************************************************//
		//                       Set関数
		//_***************************************************//

		//名前のセット
		void SetName(std::string name);
		//攻撃力のセット
		void SetAttack(float attack);
		//耐久値のセット
		void SetDurable(float durable);
		//耐久値のダメージの設定
		void SetDurableDamage(float low, float strong);
		//ウエポンのタイプをセット
		void SetWeaponType(WeaponType type);
		//ウエポンのタイプをセット
		void SetWeaponType(std::string type);
		//ウエポンのタイプを取得
		WeaponType GetWeaponType();
		//ダメージ関数
		void Damage(DamageType type, float mag);


	private:
		//名前
		std::string m_Name;
		//攻撃力
		float m_AttackParam;
		//耐久値
		float m_Durable;
		//弱攻撃の耐久ダメージ
		float m_LowDurableDamage;
		//強攻撃の耐久ダメージ
		float m_StrongDurableDamage;
		//武器の種類
		WeaponType m_Type;
	};
	//class CSVLoader {
	//public:
	//	CSVLoader();

	//	void Load(std::string fname);
	//	void Load(std::string fname, const std::function<void(std::string, float, int, int, int)>& func);
	//	void LoadWeapon(std::string paramname, WeaponTable* table);
	//	void LoadMagnification(std::string magname, WeaponTable* table);
	//	
	//};
}
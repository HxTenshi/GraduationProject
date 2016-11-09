
#pragma once
#include "main.h"
#include <vector>
#include <functional>
#include<string>



//*****************************************************//
//				WeaponMagnificationType
//*****************************************************//

enum class WeaponMagnificationType {
	Mag_One_Weak = 0,
	Mag_Two_Weak,
	Mag_Three_Weak,
	Mag_Fly_Weak,
	Mag_Strong,
	Mag_Fly_Strong,
	Mag_Attracted,
	Mag_Jumped,
	Mag_Deathblow,
	Mag_Throw
};
class WeaponCommon :public IDllScriptComponent{
	
};

namespace funifuni {
	//*****************************************************//
	//					WeaponMagnification
	//*****************************************************//

	//インスペクターで入れないでください
	class WeaponMagnification {
	public:
		WeaponMagnification();
		void AddMag(float mag);
		float GetMagnification(WeaponMagnificationType type);
		int GetLange();
	private:
		std::vector<float> m_Magnification;
	};
	//*****************************************************//
	//					WeaponParametor
	//*****************************************************//
	class WeaponParametor {
	public:
		WeaponParametor();
		~WeaponParametor();



		//倍率のテーブルから倍率の取得。
		const WeaponMagnification& GetMagnification();


		//耐久値へのダメージ
		void DurableDamage(WeaponMagnificationType type);
		//耐久値が無くなったか
		bool isBreak();
		//耐久値の取得
		int GetDurable();

		//攻撃力の取得
		float AttackParam();
		//攻撃時のダメージの取得
		float AttackDamage(WeaponMagnificationType type);

		//****************************************************//
		//                       Set関数
		//_***************************************************//

		//名前のセット
		void SetName(std::string name);
		//攻撃力のセット
		void SetAttack(float attack);
		//倍率のテーブルの参照を変数に格納
		void SetMagnification(const WeaponMagnification& mag);
		//耐久値のセット
		void SetDurable(int durable);
		//耐久値のダメージの設定
		void SetDurableDamage(int low, int strong);

		void SetAllParam(std::string name, float attack, int durable, int low, int strong);
	private:
		bool isOverLange(WeaponMagnificationType type);

	private:
		//名前
		std::string m_Name;
		//倍率
		WeaponMagnification m_Magnification;
		//攻撃力
		float m_AttackParam;
		//耐久値
		int m_Durable;
		//弱攻撃の耐久ダメージ
		int m_LowDurableDamage;
		//強攻撃の耐久ダメージ
		int m_StrongDurableDamage;

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
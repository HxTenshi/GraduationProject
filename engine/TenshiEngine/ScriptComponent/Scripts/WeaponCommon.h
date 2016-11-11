
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
	//					WeaponParametor
	//*****************************************************//
	class WeaponParametor {
	public:
		WeaponParametor();
		~WeaponParametor();


		//�ϋv�l�ւ̃_���[�W
		void DurableDamage(WeaponMagnificationType type);
		//�ϋv�l�������Ȃ�����
		bool isBreak();
		//�ϋv�l�̎擾
		int GetDurable();

		//�U���͂̎擾
		float AttackParam();

		//****************************************************//
		//                       Set�֐�
		//_***************************************************//

		//���O�̃Z�b�g
		void SetName(std::string name);
		//�U���͂̃Z�b�g
		void SetAttack(float attack);
		//�ϋv�l�̃Z�b�g
		void SetDurable(int durable);
		//�ϋv�l�̃_���[�W�̐ݒ�
		void SetDurableDamage(int low, int strong);

		void SetAllParam(std::string name, float attack, int durable, int low, int strong);

	private:
		//���O
		std::string m_Name;
		//�U����
		float m_AttackParam;
		//�ϋv�l
		int m_Durable;
		//��U���̑ϋv�_���[�W
		int m_LowDurableDamage;
		//���U���̑ϋv�_���[�W
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
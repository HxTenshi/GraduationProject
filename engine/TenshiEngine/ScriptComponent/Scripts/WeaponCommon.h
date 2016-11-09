
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

	//�C���X�y�N�^�[�œ���Ȃ��ł�������
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



		//�{���̃e�[�u������{���̎擾�B
		const WeaponMagnification& GetMagnification();


		//�ϋv�l�ւ̃_���[�W
		void DurableDamage(WeaponMagnificationType type);
		//�ϋv�l�������Ȃ�����
		bool isBreak();
		//�ϋv�l�̎擾
		int GetDurable();

		//�U���͂̎擾
		float AttackParam();
		//�U�����̃_���[�W�̎擾
		float AttackDamage(WeaponMagnificationType type);

		//****************************************************//
		//                       Set�֐�
		//_***************************************************//

		//���O�̃Z�b�g
		void SetName(std::string name);
		//�U���͂̃Z�b�g
		void SetAttack(float attack);
		//�{���̃e�[�u���̎Q�Ƃ�ϐ��Ɋi�[
		void SetMagnification(const WeaponMagnification& mag);
		//�ϋv�l�̃Z�b�g
		void SetDurable(int durable);
		//�ϋv�l�̃_���[�W�̐ݒ�
		void SetDurableDamage(int low, int strong);

		void SetAllParam(std::string name, float attack, int durable, int low, int strong);
	private:
		bool isOverLange(WeaponMagnificationType type);

	private:
		//���O
		std::string m_Name;
		//�{��
		WeaponMagnification m_Magnification;
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
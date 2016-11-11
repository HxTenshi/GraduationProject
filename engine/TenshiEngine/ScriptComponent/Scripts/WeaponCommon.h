
#pragma once
#include "main.h"
#include <vector>
#include <functional>
#include<string>



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
		//�E�G�|���̃^�C�v���Z�b�g
		void SetWeaponType(WeaponType type);
		//�E�G�|���̃^�C�v���Z�b�g
		void SetWeaponType(std::string type);
		//�E�G�|���̃^�C�v���擾
		WeaponType GetWeaponType();
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
		//����̎��
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

#pragma once
#include "main.h"


class Weapon :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
//�ǉ��֐�
	void Damage(int damage);
	bool isBreak();
	void ThrowAway();
	void ThrowAway(XMVECTOR& throwdir);

	void GetWeapon();



private:
	//�����o�ϐ�
	//�ϋv�l
	SERIALIZE
	int m_Endurance;

	XMVECTOR m_throw_dir;
	XMVECTOR m_frame_pos;
};
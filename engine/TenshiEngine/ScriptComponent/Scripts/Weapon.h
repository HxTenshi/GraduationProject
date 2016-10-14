
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
//追加関数
	void Damage(int damage);
	bool isBreak();
	void ThrowAway();
	void ThrowAway(XMVECTOR& throwdir);

	void GetWeapon();



private:
	//メンバ変数
	//耐久値
	SERIALIZE
	int m_Endurance;
	SERIALIZE
	int m_AttackForce;
	SERIALIZE
	float m_Recast;
};
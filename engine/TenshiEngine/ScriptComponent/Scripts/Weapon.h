
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
//’Ç‰ÁŠÖ”
	void Damage(int damage);
	bool isBreak();
	void ThrowAway();
	void ThrowAway(XMVECTOR& throwdir);

	void GetWeapon();



private:
	//ƒƒ“ƒo•Ï”
	//‘Ï‹v’l
	SERIALIZE
	int m_Endurance;
	SERIALIZE
	int m_AttackForce;
	SERIALIZE
	float m_Recast;
};
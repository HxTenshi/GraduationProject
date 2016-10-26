
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
//í«â¡ä÷êî
	void Damage(int damage);
	bool isBreak();
	void ThrowAway();
	void ThrowAway(XMVECTOR& throwdir);

	void WeaponUsePhysX();
	void GetWeapon();
private:
	void ThrowAwayAction();
	void PierceSupport(GameObject obj);

private:
	//ÉÅÉìÉoïœêî
	//ëœãvíl
	SERIALIZE
	int m_Endurance;
	SERIALIZE
	int m_AttackForce;
	SERIALIZE
	float m_Recast;
	SERIALIZE
	GameObject m_ThrowHit;

private:
	float m_weapon_rot;
	bool is_hand;
	bool is_ground_hit;
};
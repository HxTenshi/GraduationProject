
#pragma once
#include "main.h"
# include "WeaponControl.h"
enum AttackType {
	WEAK,
	USUALLY,
	STRONG
};
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
	void ThrowAttack();
	void ThrowAway(XMVECTOR& throwdir);

	void WeaponUsePhysX();
	void GetWeapon();
	void Attack();
private:
	void ThrowAwayAction();
	void PierceSupport(GameObject obj);
//演出関係
private:
	void Break();
	void Effect();
private:
	//メンバ変数
	//耐久値
	SERIALIZE
	int m_Endurance;
	SERIALIZE
	int m_AttackForce;
	SERIALIZE
	float m_Recast;
	SERIALIZE
	GameObject m_ThrowHit;

	//鈴木追加
	SERIALIZE GameObject mWeaponControl;
	bool mIsEnemyThrow;
	AttackType m_attack_type;
private:
	float m_weapon_rot;
	bool is_hand;
	bool is_ground_hit;
};
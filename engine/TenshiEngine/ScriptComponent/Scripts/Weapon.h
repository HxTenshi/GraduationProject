
#pragma once
#include "main.h"
# include "WeaponControl.h"
#include <functional>
enum AttackType {
	WEAK,
	USUALLY,
	STRONG
};
class Weapon :public IDllScriptComponent{
public:
	struct HitState {
		enum Type {
			Damage,
			Guard,
			Counter,
			Dogde
		};
	};
	typedef std::function<void(GameObject, Weapon*, Weapon::HitState::Type)> HitCollbackType;

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
	void ThrowAttack();
	void ThrowAway(XMVECTOR& throwdir);

	void WeaponUsePhysX();
	/// <summary>
	///�U�����̃R�[���o�b�N�֐��ǉ�
	/// </summary>
	void SetHitCollback(const HitCollbackType& collback);
	void GetWeapon();
	void Attack();
	float GetAttackPower() {
		return m_AttackForce;
	}
private:
	void ThrowAwayAction();
	void PierceSupport(GameObject obj);
//���o�֌W
private:
	void Break();
	void Effect();
private:
	//�����o�ϐ�
	//�ϋv�l
	SERIALIZE
	int m_Endurance;
	SERIALIZE
	int m_AttackForce;
	SERIALIZE
	float m_Recast;
	SERIALIZE
	GameObject m_ThrowHit;

	//��ؒǉ�
	SERIALIZE GameObject mWeaponControl;
	bool mIsEnemyThrow;
	AttackType m_attack_type;
private:
	//����̃p�����[�^

private:
	float m_weapon_rot;
	bool is_hand;
	bool is_ground_hit;

	HitCollbackType m_HitCollback;
};
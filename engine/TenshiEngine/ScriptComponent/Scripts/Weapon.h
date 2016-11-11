
#pragma once
#include "main.h"
# include "WeaponControl.h"
#include "WeaponCommon.h"
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
	/// <summary>
	///����ւ̃_���[�W����
	/// </summary>
	void Damage(int damage);
	/// <summary>
	///��ꂽ��
	/// </summary>
	bool isBreak();
	/// <summary>
	///�����̂Ă�
	/// </summary>
	void ThrowAway();
	/// <summary>
	///�����̂Ă�
	/// </summary>
	void ThrowAttack();
	/// <summary>
	///�����̂Ă�
	/// </summary>
	void ThrowAway(XMVECTOR& throwdir);
	/// <summary>
	///Weapon��PhysX��L���ɂ���
	/// </summary>
	void WeaponUsePhysX();
	/// <summary>
	///�U�����̃R�[���o�b�N�֐��ǉ�
	/// </summary>
	void SetHitCollback(const HitCollbackType& collback);
	/// <summary>
	///����̎擾
	/// </summary>
	void GetWeapon();
	/// <summary>
	///�U��
	/// </summary>
	void Attack();
	/// <summary>
	///�U���͂̎擾
	/// </summary>
	float GetAttackPower();
	/// <summary>
	///�ϋv�͂̎擾
	/// </summary>
	float GetDurable();
	/// <summary>
	///����̎̂ĕ��̎�ނ��w��
	/// </summary>
	WeaponType GetWeaponType();
	/// <summary>
	///����̌�������
	/// </summary>
	void SwapWeapon(GameObject target);
private:
	/// <summary>
	///�����̂Ă鎞�̃A�N�V����
	/// </summary>
	void ThrowAwayAction();
	void PierceSupport(GameObject obj);
//���o�֌W
private:
	void Break();
	void Effect();
private:
	//�����o�ϐ�
	SERIALIZE
	float m_Recast;
	SERIALIZE
	GameObject m_ThrowHit;

	//��ؒǉ�
	SERIALIZE GameObject mWeaponControl;
	bool mIsEnemyThrow;
	AttackType m_attack_type;

	//����̃p�����[�^
	funifuni::WeaponParametor m_param;
private:
	float m_weapon_rot;
	bool is_hand;
	bool is_ground_hit;

	HitCollbackType m_HitCollback;
};
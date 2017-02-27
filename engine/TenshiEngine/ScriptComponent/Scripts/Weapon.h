
#pragma once
#include "main.h"
# include "WeaponControl.h"
#include "WeaponCommon.h"
#include "WeaponTable.h"
#include "WeaponEffect.h"
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
	void Damage(DamageType type,float mag);
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
	///������̂Ă鏈��
	/// </summary>
	void Weapon::ThrowAway(GameObject target, float speed);
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
	/// <summary>
	///������擾�ł��邩
	/// </summary>
	bool isGetWeapon();
	/// <summary>
	///����̍ő�̑ϋv�l���擾
	/// </summary>
	float GetMaxDurable();
	/// <summary>
	///�U�����Ă��邩
	/// </summary>
	bool isAttack();
	/// <summary>
	///�U���t���O�̃Z�b�g
	/// </summary>
	void SetAttackFlag(bool flag);
	/// <summary>
	///�U���t���O�̃Z�b�g
	/// </summary>
	void SetAttackFlag(int flag);
	/// <summary>
	///��Ɏ����Ă��邩
	/// </summary>
	bool GetIsHand();
	/// <summary>
	///����𓊂��Ă��邩
	/// </summary>
	bool isThrow();
	/// <summary>
	///�{�[���Ǐ]�^�[�Q�b�g�w��
	/// </summary>
	void SetMirrorTarget(GameObject target);
	/// <summary>
	///�{�[���Ǐ]�^�[�Q�b�g
	/// </summary>
	GameObject GetMirrorTarget();
	/// <summary>
	///���O�̎擾
	/// </summary>
	std::string GetName();
	/// <summary>
	///�O�Ղ̐���
	/// </summary>
	void CreateEffect(WeaponEffectType type=WeaponEffectType::STRONGEF);
	/// <summary>
	///�������p�����[�^�̎擾
	/// </summary>
	funifuni::WeaponParametor GetWeaponParam();
	/// <summary>
	///�O�Ղ̃G�t�F�N�g��LifeTime��ύX����֐�
	/// </summary>
	void SetLifeTime(float t, float dtime = 0.5f);


private:
	/// <summary>
	///�����̂Ă鎞�̃A�N�V����
	/// </summary>
	void ThrowAwayAction();
	void PierceSupport(GameObject obj);
//���o�֌W
private:
	//���킪���S�ɏ�������
	void BreakWeapon();
	void Effect();
private:
	//�����o�ϐ�
	SERIALIZE
	std::string m_name;
	SERIALIZE
	float m_Recast;
	SERIALIZE
	GameObject m_ThrowHit;
	SERIALIZE
	GameObject m_WeaponEffect;
	SERIALIZE
	GameObject m_Break_Mesh;
	SERIALIZE
		PrefabAsset m_Break_Particle;
	//��ؒǉ�
	SERIALIZE GameObject mWeaponControl;
	SERIALIZE PrefabAsset m_break_effect;
	bool mIsEnemyThrow;
	AttackType m_attack_type;

	//���퓊�����ɗU������Ώ�
	GameObject m_ThrowTarget;
	float m_ThrowSpeed;
	GameObject m_MirrorTarget;

	//�����ׂ�����
	XMVECTOR m_Vector;

	//����̃p�����[�^
	funifuni::WeaponParametor m_param;
private:
	//�e�X�g�p
	SERIALIZE
	GameObject mSwapTarget;
	SERIALIZE
	bool is_break_weapon;
	SERIALIZE
	float weapon_break;
private:
	float m_weapon_rot;
	bool is_hand;
	bool is_ground_hit;
	//�̂Ă鏈�����ς�鎞�ɖ����Ȃ�
	bool is_fly;
	int is_attack;

	HitCollbackType m_HitCollback;

	

	float break_time;

	
};
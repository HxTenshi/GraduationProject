
#pragma once
#include "main.h"


class reis :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	void WeaponCallHit();

	struct MoveMode {
		enum Enum {
			Idle,
			In,
			Out,
			Count
		};
	};

	struct DogdeMode {
		enum Enum {
			Back,
			Rotate,
			Count
		};
	};

	void BattleStart();

private:
	void Teleport(const XMVECTOR& vect);
	void Move(const XMVECTOR& vect);
	void Rotate(const XMVECTOR& vect);
	XMVECTOR GetPlayerVect();
	XMVECTOR GetPlayerVectH();
	float GetPlayerLen();
	float GetPlayerLenH();
	float GetPlayerLenV();
	float GetHp();

	void OnDamage();

	void SetWeapon(bool enable,float damage = 0);
	void SetSuperArmor(bool f);

	void ChangeAnime(int id);

	bool IsCurrentAnimeEnd();

	void WeaponEffect(int id, bool enable);

	void BoneMoveUpdate();

	//メンバ変数
	SERIALIZE bool _m_DefaultStandby;

	SERIALIZE GameObject m_AnimeModel;
	SERIALIZE float m_MoveSpeed;
	SERIALIZE PrefabAsset m_Bullets;
	SERIALIZE PrefabAsset m_CitrusBullet;
	SERIALIZE PrefabAsset m_SonicWaveV;

	SERIALIZE GameObject m_WeaponEffect;
	SERIALIZE GameObject m_Weapon;
	
	//本体
	SERIALIZE GameObject m_EnemyBase;

	//effect
	SERIALIZE PrefabAsset m_DamageParticle;
	SERIALIZE PrefabAsset m_DeadParticle;
	SERIALIZE PrefabAsset m_WarpParticle;
	SERIALIZE PrefabAsset m_ChargeParticle;
	SERIALIZE PrefabAsset m_BigBangParticle;

	//ボーン追従周り
	SERIALIZE GameObject m_BoneMirrorObject;
	XMVECTOR m_BoneBackPos;
	
	//状態まわり
	bool m_SuperArmor;
	int m_SuperArmorHit;
	bool m_NowAttack;
	float m_MoveCooldown;
	int m_WinceCount;

	MoveMode::Enum m_MoveMode;

	//回避
	bool m_AutoDogdeMode;
	XMVECTOR m_DodgeBaseVect;
	DogdeMode::Enum m_DogdeMode;

	//CitrusBullet
	GameObject m_CitrusBulletObject;

	GameObject m_SonicWaveObject;

	GameObject m_Player;
	int m_ReisMode;
	int m_ReisLastAttackMode;
	int m_CurrentAnimeID;

	struct AttackStage
	{
		float time = 0.0f;
		int stage = 0;
	} m_AttackStage;
	std::vector<std::function<void(void)>> m_MoveFunc;

};
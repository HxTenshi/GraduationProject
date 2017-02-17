
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


	SERIALIZE GameObject m_AnimeModel;
	SERIALIZE float m_MoveSpeed;
	SERIALIZE PrefabAsset m_Bullets;
	SERIALIZE PrefabAsset m_CitrusBullet;
	SERIALIZE PrefabAsset m_SonicWaveV;

	SERIALIZE GameObject m_WeaponEffect;
	
	//本体
	SERIALIZE GameObject m_EnemyBase;

	SERIALIZE PrefabAsset m_DeadParticle;

	//ボーン追従周り
	SERIALIZE GameObject m_BoneMirrorObject;
	XMVECTOR m_BoneBackPos;
	
	//状態まわり
	bool m_SuperArmor;
	float m_SaveHp;
	bool m_NowAttack;
	float m_MoveCooldown;
	MoveMode::Enum m_MoveMode;


	//CitrusBullet
	GameObject m_CitrusBulletObject;
	int m_CitrusBulletCount;

	GameObject m_SonicWaveObject;

	GameObject m_Player;
	int m_ReisMode;
	int m_ReisLastAttackMode;
	int m_CurrentAnimeID;

	struct AttackStage
	{
		int stage = 0;
	} m_AttackStage;
	std::vector<std::function<void(void)>> m_MoveFunc;

};
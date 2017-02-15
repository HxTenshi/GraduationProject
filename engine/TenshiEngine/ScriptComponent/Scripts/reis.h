
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

private:
	void Teleport(const XMVECTOR& vect);
	void Move(const XMVECTOR& vect);
	void Rotate(const XMVECTOR& vect);
	XMVECTOR GetPlayerVect();
	XMVECTOR GetPlayerVectH();
	float GetPlayerLen();
	float GetPlayerLenH();
	float GetPlayerLenV();

	void ChangeAnime(int id);

	bool IsCurrentAnimeEnd();

	void WeaponEffect(int id, bool enable);

	void BoneMoveUpdate();

	//ÉÅÉìÉoïœêî


	SERIALIZE GameObject m_AnimeModel;
	SERIALIZE float m_MoveSpeed;
	SERIALIZE PrefabAsset m_Bullets;
	SERIALIZE PrefabAsset m_CitrusBullet;
	SERIALIZE PrefabAsset m_SonicWaveV;

	SERIALIZE GameObject m_WeaponEffect;
	
	//É{Å[Éìí«è]é¸ÇË
	SERIALIZE GameObject m_BoneMirrorObject;
	XMVECTOR m_BoneBackPos;
	

	GameObject m_CitrusBulletObject;
	int m_CitrusBulletCount;
	float m_CitrusBulletCollDown;

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
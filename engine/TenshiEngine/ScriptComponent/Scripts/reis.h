
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
	void Move(const XMVECTOR& vect);
	void Rotate(const XMVECTOR& vect);
	XMVECTOR GetPlayerVect();
	float GetPlayerLen();
	float GetPlayerLenH();
	float GetPlayerLenV();

	void ChangeAnime(int id);

	bool IsCurrentAnimeEnd();

	//メンバ変数


	SERIALIZE GameObject m_AnimeModel;
	SERIALIZE float m_MoveSpeed;
	SERIALIZE PrefabAsset m_Bullets;


	GameObject m_Player;
	int m_ReisMode;
	int m_CurrentAnimeID;
};
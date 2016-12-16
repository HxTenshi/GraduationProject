
#pragma once
#include "main.h"


class WeaponEffect :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;

	//アクションの起動処理
	void Action();
	//アクションの実行中の処理
	void Play();
	//ライフタイムのリセット
	void Reset();
	//存在しているか
	bool isAlive();
	//時間が過ぎると削除する
	void DeleteAction();

private:
	//メンバ変数
	SERIALIZE
	PrefabAsset m_particle;
	SERIALIZE
	GameObject m_now_Effect;
	SERIALIZE
	float m_life_time;
	float m_now_life_time;
	

};
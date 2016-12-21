
#pragma once
#include "main.h"


class WeaponEffect :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;

	//アクションの起動処理1回で一つ生成
	void Action();
	//アニメーションの違いによるタイムのセット
	void SetLifeTime(float time,float dtime=0.5f);

private:
	//メンバ変数
	SERIALIZE
	PrefabAsset m_particle;
	SERIALIZE
	GameObject m_now_Effect;
	SERIALIZE
	float m_life_time;
	SERIALIZE
	float m_stop_particle_time;


	

};
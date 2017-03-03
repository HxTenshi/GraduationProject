
#pragma once
#include "main.h"

enum WeaponEffectType{
	STRONGEF,
	THROWEF
};
class WeaponEffect :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;

	//アクションの起動処理1回で一つ生成
	void Action(WeaponEffectType type = WeaponEffectType::STRONGEF);
	void DeleteParticle(float time, float dtime);
	//投げる場合は長い時間をセットして刺さったときにもう一度タイムをセットして
	//アニメーションの違いによるタイムのセット
	void SetLifeTime(float time,float dtime=0.5f);
private:
	//メンバ変数
	SERIALIZE
	PrefabAsset m_particle;
	SERIALIZE
	PrefabAsset m_throw_particle;
	SERIALIZE
	GameObject weaponTip;
	SERIALIZE
	float m_life_time;
	SERIALIZE
	float m_stop_particle_time;

	GameObject m_nowParticle;

	

};
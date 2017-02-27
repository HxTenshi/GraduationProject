
#pragma once
#include "main.h"


class WeaponEffectCtr :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void SetLifeTime(float time,float stoptime);
	void Reset();
private:
	//メンバ変数
	float m_lifetime;
	float m_now_lifetime;
	float m_stop_particle_time;
};
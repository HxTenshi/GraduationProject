
#pragma once
#include "main.h"
#include "Weapon.h"

class Bomb :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void CountDown();
	void Explosion();
private:
	//ÉÅÉìÉoïœêî
	SERIALIZE
		GameObject m_weapon_object;
	SERIALIZE
		float m_explosion_time;
	SERIALIZE
		PrefabAsset m_Explosion_pref;
	bool is_start;
	bool started_flag;
};
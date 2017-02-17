
#pragma once
#include "main.h"


class EnemyActionEffectDamage :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE float m_create_time;
	SERIALIZE float m_damage;
	SERIALIZE float m_delete_time;
	bool m_hit;
	float m_timer;
};
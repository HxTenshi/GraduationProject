
#pragma once
#include "main.h"


class PillerTrap :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void Check();
	void Action();
	void Retract();
private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE float m_damage;
	SERIALIZE float m_action_time;

	float m_time;

	bool is_action;
	const float up = 1.5f;
	const float down = 0.0f;
};
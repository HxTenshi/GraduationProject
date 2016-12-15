
#pragma once
#include "main.h"


class FireEffect :public IDllScriptComponent{
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
	SERIALIZE float m_MaxHDR;
	SERIALIZE float m_MinHDR;
	SERIALIZE float m_RandomPower;
	float m_Timer;
};
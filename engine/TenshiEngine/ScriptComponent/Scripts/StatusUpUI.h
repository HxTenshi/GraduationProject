
#pragma once
#include "main.h"


class StatusUpUI :public IDllScriptComponent{
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
	SERIALIZE float m_UpTime;
	SERIALIZE float m_AlphaTime;
	SERIALIZE float m_UpY;
	float m_UpTimer;
	float m_AlphaTimer;
	float m_SetY;
};
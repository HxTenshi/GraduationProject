
#pragma once
#include "main.h"


class ReisSonicWave :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;


	void PlayerLockOn();
private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE PrefabAsset m_WaveEffect;
	SERIALIZE GameObject m_HitBox;

	SERIALIZE float m_WaitTime;

	bool m_GenEnd;
	float m_Timer;
	int m_HitOnFrame;
};
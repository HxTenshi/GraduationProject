
#pragma once
#include "main.h"


class ReisBullet :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

private:
	void PlayerLockOn();
	//ÉÅÉìÉoïœêî
	SERIALIZE float m_Speed;
	SERIALIZE float m_WaitTime;
	SERIALIZE bool m_PlayerTarget;
	SERIALIZE float m_DestroyWaitTime;
	SERIALIZE PrefabAsset m_DestroyParticle;
	SERIALIZE GameObject m_BulletModel;
	bool m_IsDestroy;
	float m_AliveTimer;
	float m_DestroyWaitTimer;
	XMVECTOR m_Vector;
};
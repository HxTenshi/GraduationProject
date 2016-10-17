
#pragma once
#include "main.h"


class WeaponHand :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;


	void SetWeapon(GameObject weapon);

	void Attack();
	void Guard();

	void ThrowAway();
private:
	//ÉÅÉìÉoïœêî
	GameObject mWeapon;

	SERIALIZE
	float m_GetSpeed;
	SERIALIZE
	float m_GetRotSpeed;
	SERIALIZE
	float m_WaveHeight;
	float m_Wave;
	bool m_NowGetAction;
	float m_GetPosDistance;

	bool m_ActionFree;

	SERIALIZE
	GameObject m_GuardPos;
	float m_AttackTime;
	bool m_IsGuard;
};
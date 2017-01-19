
#pragma once
#include "main.h"
#include "Weapon.h"

class WeaponHand :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	bool ActionFree() {
		return m_ActionFree;
	}

	GameObject GetHandWeapon();
	void SetWeapon(GameObject weapon, const Weapon::HitCollbackType& collback);

	void ThrowAway();
	void ThrowAway(XMVECTOR vector);
	void ThrowAway(GameObject target,bool isMove);
private:
	//ÉÅÉìÉoïœêî
	GameObject mWeapon;

	SERIALIZE
	float m_ThrowPower;
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
};
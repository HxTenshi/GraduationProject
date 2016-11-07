
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

	void LowAttack_1();
	void LowAttack_2();
	void LowAttack_3();
	void HighAttack_1();
	void HighAttack_2();
	void FloatLowAttack_1();
	void SpecialAttack();
	void Guard();

	void ThrowAway();
	void ThrowAway(XMVECTOR vector);
	void ThrowAway(GameObject target,bool isMove);
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

	std::function<void(void)> m_AttackFunction;
};
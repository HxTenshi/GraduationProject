
#pragma once
#include "main.h"


class BombDamageEffect :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	void SetDamage(float value);
	void SetKnockBackPower(float value);
	float GetDamage();
	float GetKnockBackPower();

private:
	//ÉÅÉìÉoïœêî
	SERIALIZE float m_Damage;
	SERIALIZE float m_KnockBackPower;
	SERIALIZE bool m_DontPlaySE;
	bool m_PlaySE;
	bool m_dead;
};
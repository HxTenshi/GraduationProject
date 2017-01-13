
#pragma once
#include "main.h"


class BurstDamageArea :public IDllScriptComponent{
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
	SERIALIZE float m_Damege;
	SERIALIZE bool m_KnockBackDonw;
	SERIALIZE bool m_DodgeInevitable;
	SERIALIZE bool m_GuardInevitable;
};
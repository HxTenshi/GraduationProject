
#pragma once
#include "main.h"


class BreakForce :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

private:
	//ÉÅÉìÉoïœêî
	SERIALIZE
		GameObject m_Object1;
	SERIALIZE
		GameObject m_Object2;
	SERIALIZE
		GameObject m_Object3;
	SERIALIZE
		GameObject m_Object4;
	SERIALIZE
		GameObject m_Object5;
	SERIALIZE
		GameObject m_Object6;
	SERIALIZE
		float m_Force;
};
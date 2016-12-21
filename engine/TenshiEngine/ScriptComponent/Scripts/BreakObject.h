
#pragma once
#include "main.h"


class BreakObject :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

private:
	//�����o�ϐ�

	SERIALIZE
	GameObject m_NormalObject;
	SERIALIZE
	GameObject m_BreakObject;
};
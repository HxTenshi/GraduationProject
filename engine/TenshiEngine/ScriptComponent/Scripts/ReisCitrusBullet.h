
#pragma once
#include "main.h"


class ReisCitrusBullet :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	void SetVector(const XMVECTOR& vect);

private:
	//メンバ変数
	SERIALIZE float m_Speed;
	SERIALIZE float m_RotateSpeed;
	float m_SE_Timer;
	XMVECTOR m_Vector;
};

#pragma once
#include "main.h"


class Item :public IDllScriptComponent{
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

	SERIALIZE float m_AddHP;
	SERIALIZE float m_AddMaxHP;
	SERIALIZE float m_AddSP;
	SERIALIZE
	PrefabAsset m_GetEffect;


	SERIALIZE float m_RotY;
	SERIALIZE float m_MoveY;
	SERIALIZE float m_MoveY_Time;
	float m_Time;
	XMVECTOR m_StartPos;
};
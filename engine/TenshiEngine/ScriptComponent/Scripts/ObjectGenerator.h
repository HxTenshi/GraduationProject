
#pragma once
#include "main.h"


class ObjectGenerator :public IDllScriptComponent{
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

	SERIALIZE float m_Time;
	SERIALIZE int m_Num;
	SERIALIZE float m_GeneratPercent;
	SERIALIZE PrefabAsset m_Object;
	SERIALIZE bool m_UniqueGeneratMode;
	SERIALIZE bool m_RotateCopy;
	GameObject m_GeneratObject;
	float m_Timer;
	int m_Count;
};
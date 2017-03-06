
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
	GameObject GetGeneratorObject() { return m_GeneratObject; }
private:
	//�����o�ϐ�

	SERIALIZE float m_Time;
	SERIALIZE int m_Num;
	SERIALIZE float m_GeneratPercent;
	SERIALIZE PrefabAsset m_Object;
	SERIALIZE bool m_UniqueGeneratMode;
	SERIALIZE bool m_RotateCopy;
	SERIALIZE bool m_PositionCopyNot;

	SERIALIZE GameObject m_OutputGimick;
	SERIALIZE GameObject m_DestroyOutputGimick;
	GameObject m_GeneratObject;
	float m_Timer;
	int m_Count;
	bool m_GeneratFlag;
};
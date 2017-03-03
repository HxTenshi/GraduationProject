
#pragma once
#include "main.h"


class OutputTextureUI :public IDllScriptComponent{
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
	SERIALIZE float m_WaitTime;
	SERIALIZE float m_TimeScale;
	SERIALIZE GameObject m_EndAction;
	float m_Timer;
	bool m_FirstFrame;
};
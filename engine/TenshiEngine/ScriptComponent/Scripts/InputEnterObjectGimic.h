
#pragma once
#include "main.h"


class InputEnterObjectGimic :public IDllScriptComponent{
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
	SERIALIZE GameObject _m_OutputGimic;

	SERIALIZE GameObject m_TargetObj;
	SERIALIZE std::string m_TargetName;
	SERIALIZE int m_TargetLayer;
	SERIALIZE std::string m_TargetScript;
};
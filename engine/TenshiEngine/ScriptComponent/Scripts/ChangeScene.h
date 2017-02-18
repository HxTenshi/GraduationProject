
#pragma once
#include "main.h"


class ChangeScene :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	void Next();

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE std::string m_SceneName;
};
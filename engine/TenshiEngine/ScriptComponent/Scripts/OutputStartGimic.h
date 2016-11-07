
#pragma once
#include "main.h"


class OutputStartGimic :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	bool OnStart();

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE
	bool m_StartMatineeMode;

	SERIALIZE
	GameObject m_Output;
};
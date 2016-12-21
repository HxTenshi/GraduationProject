
#pragma once
#include "main.h"


class InputDestroyGimic :public IDllScriptComponent{
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
	bool m_End;

	SERIALIZE
		GameObject m_Target;
	SERIALIZE
	GameObject m_OutputGimic;
};
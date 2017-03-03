
#pragma once
#include "main.h"


class Tutrial_1_5_Program :public IDllScriptComponent{
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
	SERIALIZE GameObject m_Mogitou;
	SERIALIZE GameObject m_HintDraw;
	bool m_ComboClear;
};
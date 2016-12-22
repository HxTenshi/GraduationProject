
#pragma once
#include "main.h"


class TitleManager :public IDllScriptComponent{
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
	GameObject m_GameStartButton;
	GameObject m_ConfigButton;
	GameObject m_GameEndButton;

	GameObject m_Buttons;

	SERIALIZE
	GameObject m_Canvas;
};
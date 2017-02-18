
#pragma once
#include "main.h"


class GameOverScene :public IDllScriptComponent{
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
	SERIALIZE
		GameObject m_texRogo;
	SERIALIZE
		GameObject m_texMessage;
	SERIALIZE
		GameObject m_fader;

	float m_intervalTime;
};
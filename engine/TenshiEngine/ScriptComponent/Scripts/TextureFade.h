
#pragma once
#include "main.h"


class TextureFade :public IDllScriptComponent{
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
	float m_Fade = 1.0f;
	SERIALIZE float m_FadeSpeed;
	float m_StartTime = 0;
};
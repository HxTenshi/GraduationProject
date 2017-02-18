
#pragma once
#include "main.h"
#include <h_component.h>
#include <h_standard.h>
#include <string>


class Fader :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void OnSceneChnage(std::string nextScenePass);

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE
		GameObject m_blackTex;
	bool m_isFadeIn;	//ˆÃ‚­‚È‚Á‚Ä‚¢‚­‚©
	bool m_isSceneChange;	//ƒV[ƒ“‚ğ‘JˆÚ‚·‚é‚©‚Ç‚¤‚©
	std::string m_nextScenePass;
};
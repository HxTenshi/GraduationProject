
#pragma once
#include "main.h"


class UniqueObject :public IDllScriptComponent{
public:
	UniqueObject();
	void Initialize()override;
	void Finish()override;

	static GameObject GetPlayer();
	static GameObject GetCamera();

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE GameObject m_Player;
	SERIALIZE GameObject m_Camera;
};
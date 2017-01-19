
#pragma once
#include "main.h"


class UniqueObject :public IDllScriptComponent{
public:
	UniqueObject();

	void Finish()override;

	static GameObject GetPlayer();
	static GameObject GetCamera();

private:
	//メンバ変数
	SERIALIZE GameObject m_Player;
	SERIALIZE GameObject m_Camera;
};
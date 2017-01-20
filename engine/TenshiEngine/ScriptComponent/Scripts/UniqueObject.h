
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
	//�����o�ϐ�
	SERIALIZE GameObject m_Player;
	SERIALIZE GameObject m_Camera;
};
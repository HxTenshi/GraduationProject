
#pragma once
#include "main.h"


class UniqueObject :public IDllScriptComponent{
public:

	void Initialize()override;
	void Finish()override;

	static bool IsPlayer(GameObject target);
	static GameObject GetPlayer();
	static GameObject GetCamera();

private:
	//�����o�ϐ�
	SERIALIZE GameObject m_Player;
	SERIALIZE GameObject m_Camera;
};
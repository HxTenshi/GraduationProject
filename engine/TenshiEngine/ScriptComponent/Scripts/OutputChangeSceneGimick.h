
#pragma once
#include "main.h"
#include "OutputGimic.h"


class OutputChangeSceneGimick :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//�����o�ϐ�
	SERIALIZE std::string m_SceneName;
	SERIALIZE GameObject m_Fade;
};
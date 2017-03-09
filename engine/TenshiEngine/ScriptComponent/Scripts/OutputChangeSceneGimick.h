
#pragma once
#include "main.h"
#include "OutputGimic.h"


class OutputChangeSceneGimick :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE std::string m_SceneName;
	SERIALIZE GameObject m_Fade;
};

#pragma once
#include "main.h"
#include "OutputGimic.h"


class OutputChangeSceneGimick :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//メンバ変数
	SERIALIZE std::string m_SceneName;
};
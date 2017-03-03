
#pragma once
#include "main.h"
# include "OutputGimic.h"

class EventSceneChange :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//メンバ変数
	//Assets/Scene/kiminonawa.scene
	SERIALIZE std::string mPath;
	SERIALIZE GameObject mFade;
};
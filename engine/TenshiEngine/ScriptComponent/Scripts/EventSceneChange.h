
#pragma once
#include "main.h"
# include "OutputGimic.h"

class EventSceneChange :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//ƒƒ“ƒo•Ï”
	//Assets/Scene/kiminonawa.scene
	SERIALIZE std::string mPath;
	SERIALIZE GameObject mFade;
};
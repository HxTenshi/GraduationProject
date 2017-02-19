
#pragma once
#include "main.h"
#include "OutputGimic.h"

class EventWave :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;
private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE GameObject mTarget;
};
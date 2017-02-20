
#pragma once
#include "main.h"
# include "OutputGimic.h"

class OutputEventTimer :public OutputGimic {
public:
	void Update();
	bool OnStart(GameObject Sender) override;

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE GameObject mTarget;
	SERIALIZE float mWaitTime;
	float mTime;
	SERIALIZE bool mIsEnd;
	SERIALIZE bool mIsStart;
};
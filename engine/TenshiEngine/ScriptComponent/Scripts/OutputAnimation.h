
#pragma once
#include "main.h"
#include "OutputGimic.h"

class OutputAnimation :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE GameObject m_Target;
	//—LŒø‰»–³Œø‚©
	SERIALIZE int mPlayAnimationID;
	SERIALIZE float mWeight;
	SERIALIZE float mTime;
	SERIALIZE float mTimeScale;
	SERIALIZE GameObject m_Output;
};
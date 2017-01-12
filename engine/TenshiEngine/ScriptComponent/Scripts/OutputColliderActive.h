
#pragma once
#include "main.h"
#include "OutputGimic.h"

class OutputColliderActive :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE GameObject m_Target;
	//—LŒø‰»–³Œø‚©
	SERIALIZE bool mIsEnable;
	SERIALIZE GameObject m_Output;
};
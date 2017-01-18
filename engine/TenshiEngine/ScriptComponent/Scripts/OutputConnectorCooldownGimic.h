
#pragma once
#include "main.h"

#include "OutputGimic.h"

class OutputConnectorCooldownGimic :public OutputGimic {
public:
	void Initialize()override;
	void Update()override;
	bool OnStart(GameObject Sender) override;

private:
	//ƒƒ“ƒo•Ï”
	float m_Timer;
	SERIALIZE float m_CoolTime;
	SERIALIZE GameObject m_Output;
	SERIALIZE GameObject m_CoolEndOutput;
};
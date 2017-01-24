
#pragma once
#include "main.h"

#include "OutputGimic.h"

class OutputConnectorCountGimick :public OutputGimic {
public:
	void Initialize()override;
	bool OnStart(GameObject Sender) override;

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE int m_Count;
	SERIALIZE GameObject m_Output;
	SERIALIZE bool m_Loop;
	int m_Counter;
};
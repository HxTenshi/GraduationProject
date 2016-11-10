
#pragma once
#include "main.h"

#include "OutputGimic.h"

class OutputConnectorOneCollGimic :public OutputGimic {
public:
	void Initialize()override;

	bool OnStart(GameObject Sender) override;

private:
	//ƒƒ“ƒo•Ï”

	SERIALIZE
	GameObject m_OutputGimic;

	bool m_End;
};
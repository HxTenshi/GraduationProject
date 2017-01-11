
#pragma once
#include "main.h"

#include "OutputGimic.h"

class OutputConnectorToggleExporterGimic :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE bool m_On;
	SERIALIZE GameObject m_OnOutput;
	SERIALIZE GameObject m_OffOutput;
};

#pragma once
#include "main.h"

#include "OutputGimic.h"

class OutputConnectorMultipleExporterGimic :public OutputGimic{
public:
	bool OnStart(GameObject Sender) override;

private:
	//ÉÅÉìÉoïœêî

	SERIALIZE GameObject m_Output_1;
	SERIALIZE GameObject m_Output_2;
	SERIALIZE GameObject m_Output_3;
	SERIALIZE GameObject m_Output_4;
	SERIALIZE GameObject m_Output_5;
	SERIALIZE GameObject m_Output_6;
};
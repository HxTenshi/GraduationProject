
#pragma once
#include "main.h"

#include "OutputGimic.h"

class OutputConnectorToggleExporterGimic :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//�����o�ϐ�
	SERIALIZE bool m_On;
	SERIALIZE GameObject m_OnOutput;
	SERIALIZE GameObject m_OffOutput;
};
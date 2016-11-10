
#pragma once
#include "main.h"
#include "OutputGimic.h"

class OutputChangeEnableGimic :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//�����o�ϐ�
	SERIALIZE GameObject m_Target;
	SERIALIZE bool m_ToEnableMode;
	SERIALIZE bool m_ToDisableMode;
	SERIALIZE bool m_ToggleMode;
};
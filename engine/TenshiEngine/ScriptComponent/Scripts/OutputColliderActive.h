
#pragma once
#include "main.h"
#include "OutputGimic.h"

class OutputColliderActive :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//�����o�ϐ�
	SERIALIZE GameObject m_Target;
	//�L����������
	SERIALIZE bool mIsEnable;
	SERIALIZE GameObject m_Output;
};
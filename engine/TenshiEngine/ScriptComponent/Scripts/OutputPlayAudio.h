
#pragma once
#include "main.h"
#include "OutputGimic.h"

//�C�x���g���̉��Đ�
class OutputPlayAudio :public OutputGimic {
	bool OnStart(GameObject Sender) override;

private:
	//�����o�ϐ�
	SERIALIZE GameObject m_PlayTarget;
	SERIALIZE GameObject m_OutputGimic;
};
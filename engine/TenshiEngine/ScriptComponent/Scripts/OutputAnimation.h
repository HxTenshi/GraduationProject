
#pragma once
#include "main.h"
#include "OutputGimic.h"

class OutputAnimation :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//�����o�ϐ�
	SERIALIZE GameObject m_Target;
	//�L����������
	SERIALIZE int mPlayAnimationID;
	SERIALIZE float mWeight;
	SERIALIZE float mTime;
	SERIALIZE float mTimeScale;
	SERIALIZE GameObject m_Output;
};
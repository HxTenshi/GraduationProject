
#pragma once
#include "main.h"
# include "OutputGimic.h"

class OutpuChangeDeltaTimeGimick :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//�����o�ϐ�
	SERIALIZE float m_TimeScale;

};
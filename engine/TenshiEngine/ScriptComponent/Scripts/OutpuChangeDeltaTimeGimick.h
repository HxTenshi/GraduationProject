
#pragma once
#include "main.h"
# include "OutputGimic.h"

class OutpuChangeDeltaTimeGimick :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//メンバ変数
	SERIALIZE float m_TimeScale;

};
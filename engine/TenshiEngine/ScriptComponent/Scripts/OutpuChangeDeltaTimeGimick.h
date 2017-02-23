
#pragma once
#include "main.h"
# include "OutputGimic.h"

class OutpuChangeDeltaTimeGimick :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE float m_TimeScale;

};
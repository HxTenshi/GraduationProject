#include "OutpuChangeDeltaTimeGimick.h"
#include "h_standard.h"

bool OutpuChangeDeltaTimeGimick::OnStart(GameObject Sender)
{
	Hx::DeltaTime()->SetTimeScale(m_TimeScale);
	return true;
}

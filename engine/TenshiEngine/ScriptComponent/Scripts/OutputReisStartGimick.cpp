#include "OutputReisStartGimick.h"
#include "reis.h"

bool OutputReisStartGimick::OnStart(GameObject Sender)
{
	if (m_Reis) {
		if (auto r = m_Reis->GetScript<reis>()) {
			r->BattleStart();
			return true;
		}
		
	}
	return false;
}

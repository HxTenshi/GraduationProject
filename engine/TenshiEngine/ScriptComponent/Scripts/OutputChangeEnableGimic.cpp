#include "OutputChangeEnableGimic.h"

bool OutputChangeEnableGimic::OnStart(GameObject Sender)
{
	if (!m_Target)return false;

	if (m_ToEnableMode) {
		m_Target->Enable();
	}
	else if (m_ToDisableMode) {
		m_Target->Disable();
	}
	else if (m_ToggleMode) {
		bool f = !m_Target->IsEnabled();
		if (f) {
			m_Target->Enable();
		}
		else{
			m_Target->Disable();
		}
	}
	else {
		return false;
	}
	return true;
}
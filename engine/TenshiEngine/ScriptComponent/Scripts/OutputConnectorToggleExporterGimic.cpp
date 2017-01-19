#include "OutputConnectorToggleExporterGimic.h"

bool OutputConnectorToggleExporterGimic::OnStart(GameObject Sender)
{

	m_On = !m_On;

	if (m_On) {
		if (auto scr = OutputGimic::GetOutputGimic(m_OnOutput))scr->OnStart(Sender);
	}
	else {
		if (auto scr = OutputGimic::GetOutputGimic(m_OffOutput))scr->OnStart(Sender);
	}

	return true;
}

#include "OutputConnectorMultipleExporterGimic.h"

bool OutputConnectorMultipleExporterGimic::OnStart(GameObject Sender)
{
	if (auto scr = OutputGimic::GetOutputGimic(m_Output_1))scr->OnStart(Sender);
	if (auto scr = OutputGimic::GetOutputGimic(m_Output_2))scr->OnStart(Sender);
	if (auto scr = OutputGimic::GetOutputGimic(m_Output_3))scr->OnStart(Sender);
	if (auto scr = OutputGimic::GetOutputGimic(m_Output_4))scr->OnStart(Sender);
	if (auto scr = OutputGimic::GetOutputGimic(m_Output_5))scr->OnStart(Sender);
	if (auto scr = OutputGimic::GetOutputGimic(m_Output_6))scr->OnStart(Sender);
	return true;
}

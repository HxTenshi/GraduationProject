#include "OutputConnectorCooldownGimic.h"
#include "h_standard.h"

void OutputConnectorCooldownGimic::Initialize()
{
	m_Timer = 0.0f;
}

void OutputConnectorCooldownGimic::Update()
{
	if (m_Timer != 0.0f) {
		m_Timer -= Hx::DeltaTime()->GetDeltaTime();
		m_Timer = max(m_Timer, 0.0f);
		if (m_Timer == 0.0f) {

			if (auto scr = OutputGimic::GetOutputGimic(m_CoolEndOutput))scr->OnStart(gameObject);
		}
	}
}

bool OutputConnectorCooldownGimic::OnStart(GameObject Sender)
{
	if (m_Timer == 0.0f) {
		m_Timer = m_CoolTime;

		if (auto scr = OutputGimic::GetOutputGimic(m_Output))scr->OnStart(Sender);
		return true;
	}

	return false;
}

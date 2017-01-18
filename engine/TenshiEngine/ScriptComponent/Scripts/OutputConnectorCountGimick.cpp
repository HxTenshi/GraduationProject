#include "OutputConnectorCountGimick.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void OutputConnectorCountGimick::Initialize() {
	m_Counter = 0;
}

bool OutputConnectorCountGimick::OnStart(GameObject Sender)
{
	m_Counter++;
	bool f = false;

	if (m_Count <= 0) {
		return false;
	}
	if (m_Loop) {
		f = m_Counter%m_Count == 0;
	}
	else {
		f = m_Counter == m_Count;
	}

	if (f) {
		if (auto gimick = OutputGimic::GetOutputGimic(m_Output)) {
			gimick->OnStart(Sender);
		}
	}
	return true;
}
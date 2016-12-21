#include "OutputConnectorStackGimic.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void OutputConnectorStackGimic::Initialize(){

}
bool OutputConnectorStackGimic::OnStart(GameObject Sender)
{
	
	m_Count++;

	if (m_Count == m_MaxCount) {

		auto scr = OutputGimic::GetOutputGimic(m_OutputGimic);
		if (m_Loop) {
			m_Count = 0;
		}
		if (!scr)return true;

		scr->OnStart(Sender);
	}
	return true;
}
#include "OutputConnectorOneCollGimic.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void OutputConnectorOneCollGimic::Initialize(){

	m_End = false;
}

bool OutputConnectorOneCollGimic::OnStart(GameObject Sender)
{
	if (m_End)return false;

	auto scr = OutputGimic::GetOutputGimic(m_OutputGimic);
	if (!scr)return false;

	scr->OnStart(Sender);
	m_End = true;
	return true;
}

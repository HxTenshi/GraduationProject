#include "OutputPlayAudio.h"
#include "../h_standard.h"
#include "../h_component.h"

bool OutputPlayAudio::OnStart(GameObject Sender)
{
	Hx::Debug()->Log("SE�̍Đ�����");
	if (!m_PlayTarget)return false;

	auto sound = m_PlayTarget->GetComponent<SoundComponent>();
	if (!sound)return false;
	
	Hx::Debug()->Log("SE�̍Đ�");
	m_PlayTarget->GetComponent<SoundComponent>()->Play();
	
	auto scr = OutputGimic::GetOutputGimic(m_OutputGimic);
	if (!scr)return false;
	scr->OnStart(Sender);

	return true;
}

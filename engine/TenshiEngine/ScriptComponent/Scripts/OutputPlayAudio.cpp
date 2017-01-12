#include "OutputPlayAudio.h"
#include "../h_standard.h"
#include "../h_component.h"

bool OutputPlayAudio::OnStart(GameObject Sender)
{
	if (!m_PlayTarget)return false;

	if (auto sound = m_PlayTarget->GetComponent<SoundComponent>())return false;
	
	Hx::Debug()->Log("SE‚ÌÄ¶");
	m_PlayTarget->GetComponent<SoundComponent>()->Play();
	
	auto scr = OutputGimic::GetOutputGimic(m_OutputGimic);
	if (!scr)return false;
	scr->OnStart(Sender);

	return true;
}

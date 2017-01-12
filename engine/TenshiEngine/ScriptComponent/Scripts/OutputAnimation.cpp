#include "OutputAnimation.h"
#include "../h_standard.h"
#include "../h_component.h"

bool OutputAnimation::OnStart(GameObject Sender)
{
	if (!m_Target)return false;
	Hx::Debug()->Log(m_Target->Name());
	auto anime = m_Target->GetComponent<AnimationComponent>();
	if (!anime)return false;
	Hx::Debug()->Log("ƒAƒjƒ‚Ìİ’è");

	auto p = anime->GetAnimetionParam(mPlayAnimationID);
	p.mTime = mTime;
	p.mWeight = mWeight;
	p.mTimeScale = mTimeScale;
	p.mLoop = false;
	anime->SetAnimetionParam(mPlayAnimationID, p);

	auto scr = OutputGimic::GetOutputGimic(m_Output);
	if (!scr)return false;
	scr->OnStart(Sender);

	return true;
}

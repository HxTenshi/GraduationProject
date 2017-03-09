#include "OutputChangeSceneGimick.h"
#include "h_standard.h"
#include "Score.h"
#include"Fader.h"

bool OutputChangeSceneGimick::OnStart(GameObject Sender)
{
	Hx::System()->LockCursorPositionToWindowCenter(false);
	Score::Write();
	//Hx::LoadScene(m_SceneName);
	if (!m_Fade)return false;
	auto fade = m_Fade->GetScript<Fader>();
	if (!fade)return false;

	fade->OnSceneChnage(m_SceneName);
	return true;
}

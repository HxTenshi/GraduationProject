#include "OutputChangeSceneGimick.h"
#include "h_standard.h"
#include "Score.h"

bool OutputChangeSceneGimick::OnStart(GameObject Sender)
{
	Hx::System()->LockCursorPositionToWindowCenter(false);
	Score::Write();
	Hx::LoadScene(m_SceneName);
	return true;
}

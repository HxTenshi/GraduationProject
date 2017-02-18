#include "OutputChangeSceneGimick.h"
#include "h_standard.h"

bool OutputChangeSceneGimick::OnStart(GameObject Sender)
{
	Hx::System()->LockCursorPositionToWindowCenter(false);
	Hx::LoadScene(m_SceneName);
	return true;
}

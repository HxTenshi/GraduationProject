#include "EventSceneChange.h"
# include "Fader.h"

bool EventSceneChange::OnStart(GameObject Sender)
{
	auto fade = mFade->GetScript<Fader>();
	Hx::Debug()->Log("Fade�����邩�m�F");
	if (!fade)return false;

	fade->OnSceneChnage(mPath);
	Hx::Debug()->Log("Scene �̈ړ� : "+mPath);

	return true;
}

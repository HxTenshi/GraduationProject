#include "EventSceneChange.h"
# include "Fader.h"

bool EventSceneChange::OnStart(GameObject Sender)
{
	auto fade = mFade->GetScript<Fader>();
	Hx::Debug()->Log("Fade‚ª‚ ‚é‚©Šm”F");
	if (!fade)return false;

	fade->OnSceneChnage(mPath);
	Hx::Debug()->Log("Scene ‚ÌˆÚ“® : "+mPath);

	return true;
}

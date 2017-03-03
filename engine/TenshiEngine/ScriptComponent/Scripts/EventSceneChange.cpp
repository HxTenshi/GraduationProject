#include "EventSceneChange.h"
# include "Fader.h"

bool EventSceneChange::OnStart(GameObject Sender)
{
	auto fade = mFade->GetScript<Fader>();
	Hx::Debug()->Log("Fadeがあるか確認");
	if (!fade)return false;

	fade->OnSceneChnage(mPath);
	Hx::Debug()->Log("Scene の移動 : "+mPath);

	return true;
}

#include "OutputEventTimer.h"
# include "h_standard.h"
//毎フレーム呼ばれます
void OutputEventTimer::Update(){
	if (!mIsStart)return;
	if (mIsEnd)return;
	mTime += Hx::DeltaTime()->GetDeltaTime();
	if (mTime > mWaitTime) {
		Hx::Debug()->Log("Call On Timer Event");
		auto scr = OutputGimic::GetOutputGimic(mTarget);
		if(scr)scr->OnStart(gameObject);
		mIsEnd = true;
	}
}

bool OutputEventTimer::OnStart(GameObject Sender)
{
	Hx::Debug()->Log("タイマースタート");
	mIsStart = true;
	return false;
}

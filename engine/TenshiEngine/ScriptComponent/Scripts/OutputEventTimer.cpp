#include "OutputEventTimer.h"
# include "h_standard.h"
//–ˆƒtƒŒ[ƒ€ŒÄ‚Î‚ê‚Ü‚·
void OutputEventTimer::Update(){
	if (!mIsStart)return;
	if (mIsEnd)return;
	mTime += Hx::DeltaTime()->GetDeltaTime();
	if (mTime > mWaitTime) {
		auto scr = OutputGimic::GetOutputGimic(mTarget);
		if(scr)scr->OnStart(gameObject);
		mIsEnd = true;
	}
}

bool OutputEventTimer::OnStart(GameObject Sender)
{
	mIsStart = true;
	return false;
}

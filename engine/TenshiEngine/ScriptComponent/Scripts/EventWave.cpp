#include "EventWave.h"
# include "WaveCounter.h"
bool EventWave::OnStart(GameObject Sender)
{
	if (mTarget) {
		if(mTarget->GetScript<WaveCounter>())mTarget->GetScript<WaveCounter>()->AddCounter();
		return true;
	}
	return false;
}

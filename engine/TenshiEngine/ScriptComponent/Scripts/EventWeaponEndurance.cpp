#include "EventWeaponEndurance.h"
# include "Weapon.h"
# include "OutputGimic.h"

//毎フレーム呼ばれます
void EventWeaponEndurance::Update(){

	if (!mTarget)return;
	auto weapon = mTarget->GetScript<Weapon>();
	if (!weapon)return;
	if (mIsEnd)return;

	if (weapon->isBreak()) {
		Hx::Debug()->Log("武器が壊れたので通知");
		mIsEnd = true;
		auto scr = OutputGimic::GetOutputGimic(mOutput);
		if (!scr)return;
		scr->OnStart(gameObject);
	}

}

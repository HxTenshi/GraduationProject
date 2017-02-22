#include "EventWeaponEndurance.h"
# include "Weapon.h"
# include "OutputGimic.h"

//–ˆƒtƒŒ[ƒ€ŒÄ‚Î‚ê‚Ü‚·
void EventWeaponEndurance::Update(){

	if (!mTarget)return;
	auto weapon = mTarget->GetScript<Weapon>();
	if (!weapon)return;
	if (mIsEnd)return;

	if (weapon->isBreak()) {
		Hx::Debug()->Log("•Ší‚ª‰ó‚ê‚½‚Ì‚Å’Ê’m");
		mIsEnd = true;
		auto scr = OutputGimic::GetOutputGimic(mOutput);
		if (!scr)return;
		scr->OnStart(gameObject);
	}

}

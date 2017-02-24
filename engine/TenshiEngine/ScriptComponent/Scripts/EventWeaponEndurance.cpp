#include "EventWeaponEndurance.h"
# include "Weapon.h"
# include "OutputGimic.h"

//���t���[���Ă΂�܂�
void EventWeaponEndurance::Update(){

	if (!mTarget)return;
	auto weapon = mTarget->GetScript<Weapon>();
	if (!weapon)return;
	if (mIsEnd)return;

	if (weapon->isBreak()) {
		Hx::Debug()->Log("���킪��ꂽ�̂Œʒm");
		mIsEnd = true;
		auto scr = OutputGimic::GetOutputGimic(mOutput);
		if (!scr)return;
		scr->OnStart(gameObject);
	}

}

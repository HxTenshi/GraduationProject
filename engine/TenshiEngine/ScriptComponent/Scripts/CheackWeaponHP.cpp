#include "CheackWeaponHP.h"
# include "OutputGimic.h"
# include "Weapon.h"

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void CheackWeaponHP::Start(){

}

//毎フレーム呼ばれます
void CheackWeaponHP::Update(){
	if (!mWeapon)return;
	if (!mOutputEvent)return;

	auto weapon = mWeapon->GetScript<Weapon>();
	if (weapon) {
		bool isBreak = weapon->isBreak();
		if (isBreak){
			auto scr = OutputGimic::GetOutputGimic(mOutputEvent);
			if (scr)scr->OnStart(gameObject);
			gameObject->mTransform->Disable();
		}
	}

}
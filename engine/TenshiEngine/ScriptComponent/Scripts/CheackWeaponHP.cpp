#include "CheackWeaponHP.h"
# include "OutputGimic.h"
# include "Weapon.h"

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void CheackWeaponHP::Start(){

}

//���t���[���Ă΂�܂�
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
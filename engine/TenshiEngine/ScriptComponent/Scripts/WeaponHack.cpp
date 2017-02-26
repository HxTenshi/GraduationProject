#include "WeaponHack.h"
# include "Weapon.h"
//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WeaponHack::Start(){

}

//毎フレーム呼ばれます
void WeaponHack::Update(){

	if (!mWeapon)return;

	auto weapon = mWeapon->GetScript<Weapon>();
	if (weapon) {
		weapon->GetWeaponParam().SetDurable(1);
		gameObject->mTransform->Disable();
	}

}
#include "WeaponHack.h"
# include "Weapon.h"
//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponHack::Start(){

}

//���t���[���Ă΂�܂�
void WeaponHack::Update(){

	if (!mWeapon)return;

	auto weapon = mWeapon->GetScript<Weapon>();
	if (weapon) {
		weapon->GetWeaponParam().SetDurable(0);
		gameObject->mTransform->Disable();
		Hx::Debug()->Log("����̑ϋv�͂��[���ɂ��Ă���");
		Hx::DestroyObject(gameObject);
	}

}
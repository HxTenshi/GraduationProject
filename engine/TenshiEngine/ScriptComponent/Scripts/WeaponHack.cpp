#include "WeaponHack.h"
# include "Weapon.h"
# include "OutputGimic.h"
//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponHack::Start(){
	isEnd = false;
}

//���t���[���Ă΂�܂�
void WeaponHack::Update(){

	if (!mWeapon)return;

	auto weapon = mWeapon->GetScript<Weapon>();
	if (weapon) {
		if (!isEnd) {
			weapon->Damage(DamageType::BreakDamage, 1);
			isEnd = true;
		}
		if (weapon->isBreak()) {
			auto scr = OutputGimic::GetOutputGimic(mOutput);
			if (scr)scr->OnStart(gameObject);
			Hx::Debug()->Log("����̑ϋv�͂��[���ɂ��Ă���");
			Hx::DestroyObject(gameObject);
		}
	}

}
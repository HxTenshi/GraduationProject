#include "WeaponHack.h"
# include "Weapon.h"
# include "OutputGimic.h"
//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WeaponHack::Start(){
	isEnd = false;
}

//毎フレーム呼ばれます
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
			Hx::Debug()->Log("武器の耐久力をゼロにしている");
			Hx::DestroyObject(gameObject);
		}
	}

}
#include "WeaponTip.h"
#include "Weapon.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void WeaponTip::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WeaponTip::Start(){

}

//毎フレーム呼ばれます
void WeaponTip::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void WeaponTip::Finish(){

}

//コライダーとのヒット時に呼ばれます
void WeaponTip::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void WeaponTip::OnCollideEnter(GameObject target){
	(void)target;
	if (target->GetLayer() == 4) {
		if (m_weapon->GetScript<Weapon>()) {
			m_weapon->GetScript<Weapon>()->WeaponUsePhysX();
		}
	}
}

//コライダーとのロスト時に呼ばれます
void WeaponTip::OnCollideExit(GameObject target){
	(void)target;
}
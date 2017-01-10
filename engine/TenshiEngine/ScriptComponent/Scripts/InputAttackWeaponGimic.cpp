#include "InputAttackWeaponGimic.h"
#include "h_standard.h"
#include "OutputGimic.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void InputAttackWeaponGimic::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void InputAttackWeaponGimic::Start(){

}

//毎フレーム呼ばれます
void InputAttackWeaponGimic::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void InputAttackWeaponGimic::Finish(){

}

//コライダーとのヒット時に呼ばれます
void InputAttackWeaponGimic::OnCollideBegin(GameObject target){
	(void)target;
}

#include "Weapon.h"
//コライダーとのヒット中に呼ばれます
void InputAttackWeaponGimic::OnCollideEnter(GameObject target){
	if (!target)return;
	if (!m_OutputGimic)return;

	auto gimic = OutputGimic::GetOutputGimic(m_OutputGimic);
	if (!gimic)return;

	if (auto w = target->GetScript<Weapon>()) {
		if (w->isAttack()) {
			gimic->OnStart(gameObject);
			return;
		}
	}
}

//コライダーとのロスト時に呼ばれます
void InputAttackWeaponGimic::OnCollideExit(GameObject target){
	(void)target;
}
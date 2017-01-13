#include "BreakObject.h"
#include "AttackFilter.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void BreakObject::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void BreakObject::Start(){

}

//毎フレーム呼ばれます
void BreakObject::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void BreakObject::Finish(){

}
#include"Weapon.h"
//コライダーとのヒット時に呼ばれます
void BreakObject::OnCollideBegin(GameObject target){
	if (!target)return;
	auto fil = AttackFilter::Filter::eAll;
	if (m_OnlyBombBreak) {
		fil = AttackFilter::Filter::eBomb;
	}
	if (AttackFilter::IsAttack(target,fil)) {
		if (m_NormalObject) {
			m_NormalObject->Disable();
		}
		if (m_BreakObject) {
			m_BreakObject->Enable();
		}
	}
}

//コライダーとのヒット中に呼ばれます
void BreakObject::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void BreakObject::OnCollideExit(GameObject target){
	(void)target;
}
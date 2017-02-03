#include "BurstDamageArea.h"

#include "PlayerController.h"
#include "h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void BurstDamageArea::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void BurstDamageArea::Start(){

}

//毎フレーム呼ばれます
void BurstDamageArea::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void BurstDamageArea::Finish(){

}

//コライダーとのヒット時に呼ばれます
void BurstDamageArea::OnCollideBegin(GameObject target){
	if (target) {
		float time = Hx::DeltaTime()->GetDeltaTime();
		auto player = target->GetScript<PlayerController>();
		if (player) {
			auto v = target->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
			player->Damage(m_Damege, XMVector3Normalize(v), m_KnockBackDonw ? PlayerController::KnockBack::Down : PlayerController::KnockBack::Low, m_DodgeInevitable, m_GuardInevitable);
		}
	}
}

//コライダーとのヒット中に呼ばれます
void BurstDamageArea::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void BurstDamageArea::OnCollideExit(GameObject target){
	(void)target;
}
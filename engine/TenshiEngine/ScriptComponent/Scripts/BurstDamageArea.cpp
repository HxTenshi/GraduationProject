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
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void BurstDamageArea::OnCollideEnter(GameObject target){
	if (target) {
		float time = Hx::DeltaTime()->GetDeltaTime();
		auto player = target->GetScript<PlayerController>();
		if (player) {
			player->Damage(m_Damege, XMVectorZero(), m_KnockBackDonw?PlayerController::KnockBack::Down : PlayerController::KnockBack::Low, m_DodgeInevitable,m_GuardInevitable);
		}
	}
}

//コライダーとのロスト時に呼ばれます
void BurstDamageArea::OnCollideExit(GameObject target){
	(void)target;
}
#include "EnemyActionEffectDamage.h"
#include "PlayerController.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void EnemyActionEffectDamage::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void EnemyActionEffectDamage::Start(){
	m_hit = false;
}

//毎フレーム呼ばれます
void EnemyActionEffectDamage::Update(){
	m_timer += Hx::DeltaTime()->GetDeltaTime();
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void EnemyActionEffectDamage::Finish(){

}

//コライダーとのヒット時に呼ばれます
void EnemyActionEffectDamage::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void EnemyActionEffectDamage::OnCollideEnter(GameObject target){
	if (target) {
		if (m_hit)return;
		if (m_timer > m_delete_time)return;
		if (m_timer > m_create_time) {
			auto player = target->GetScript<PlayerController>();
			if (player) {
				player->Damage(m_damage, XMVectorZero(), PlayerController::KnockBack::Down, true, true);
				m_hit = true;
			}
		}
	}
}

//コライダーとのロスト時に呼ばれます
void EnemyActionEffectDamage::OnCollideExit(GameObject target){
	(void)target;
}
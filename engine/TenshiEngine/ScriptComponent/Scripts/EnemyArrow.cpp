#include "EnemyArrow.h"
#include "Enemy.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "UniqueObject.h"
#include "PlayerController.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void EnemyArrow::Initialize(){
	count = 0;
	m_Stop = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void EnemyArrow::Start(){

}

//毎フレーム呼ばれます
void EnemyArrow::Update() {
	auto player = UniqueObject::GetPlayer();
	if (!player) {
		return;
	}
	auto ps = player->GetScript<PlayerController>();
	if (!ps) {
		return;
	}
	if (ps->GetPlayerState() == PlayerController::PlayerState::Movie) {
		return;
	}
	gameObject->mTransform->WorldPosition(gameObject->mTransform->WorldPosition() + m_Vec * speed * Hx::DeltaTime()->GetDeltaTime());
	count += Hx::DeltaTime()->GetDeltaTime();
	if (count > 30.0f) {
		Hx::DestroyObject(gameObject);
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void EnemyArrow::Finish(){

}

//コライダーとのヒット時に呼ばれます
void EnemyArrow::OnCollideBegin(GameObject target){
	if (!m_Enemy)return;
	auto enemyScript = Enemy::GetEnemy(m_Enemy);
	if (!enemyScript)return;
	if (UniqueObject::IsPlayer(target) && !m_Stop) {
		enemyScript->Attack(target, COL_TYPE::NORMAL);
	}
	else if(count >= 0.1f && target->GetLayer() == 4){
		m_Vec = XMVectorSet(0, 0, 0, 0);
		m_Stop = true;
	}
}

//コライダーとのヒット中に呼ばれます
void EnemyArrow::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void EnemyArrow::OnCollideExit(GameObject target){
	(void)target;
}
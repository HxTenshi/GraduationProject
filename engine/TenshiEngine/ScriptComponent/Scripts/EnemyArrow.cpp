#include "EnemyArrow.h"
#include "Enemy.h"
#include "../h_standard.h"
#include "../h_component.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void EnemyArrow::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void EnemyArrow::Start(){

}

//毎フレーム呼ばれます
void EnemyArrow::Update(){
	gameObject->mTransform->WorldPosition(gameObject->mTransform->WorldPosition() + m_Vec * speed * Hx::DeltaTime()->GetDeltaTime());
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void EnemyArrow::Finish(){

}

//コライダーとのヒット時に呼ばれます
void EnemyArrow::OnCollideBegin(GameObject target){
	if (!m_Enemy)return;
	auto enemyScript = Enemy::GetEnemy(m_Enemy);
	if (!enemyScript)return;
	enemyScript->Attack(target,COL_TYPE::NORMAL);
}

//コライダーとのヒット中に呼ばれます
void EnemyArrow::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void EnemyArrow::OnCollideExit(GameObject target){
	(void)target;
}
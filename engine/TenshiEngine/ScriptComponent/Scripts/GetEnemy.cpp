#include "GetEnemy.h"
#include "h_standard.h"
#include "Sandbag.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void GetEnemy::Initialize(){
	m_MinEnemy = NULL;
	m_MinEnemy_Back = NULL;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void GetEnemy::Start(){

}

//毎フレーム呼ばれます
void GetEnemy::Update(){
	m_MinEnemy_Back = m_MinEnemy;
	m_MinEnemy = NULL;
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void GetEnemy::Finish(){

}

//コライダーとのヒット時に呼ばれます
void GetEnemy::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void GetEnemy::OnCollideEnter(GameObject target){
	if (!target)return;
	if (!target->GetScript<Sandbag>())return;

	auto p1 = gameObject->mTransform->WorldPosition();
	if (m_MinEnemy) {
		auto p2 = m_MinEnemy->mTransform->WorldPosition();
		auto p3 = target->mTransform->WorldPosition();
		auto l1 = XMVector3Length(p1 - p2).x;
		auto l2 = XMVector3Length(p1 - p3).x;
		if (l1 > l2) {
			m_MinEnemy = target;
		}
	}
	else {
		m_MinEnemy = target;
	}
}

//コライダーとのロスト時に呼ばれます
void GetEnemy::OnCollideExit(GameObject target){
	(void)target;
}

GameObject GetEnemy::GetMinEnemy()
{
	return m_MinEnemy ? m_MinEnemy : m_MinEnemy_Back ? m_MinEnemy_Back : NULL;
}

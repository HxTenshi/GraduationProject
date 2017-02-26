#include "GetEnemy.h"
#include "h_standard.h"
#include "Enemy.h"
#include "UniqueObject.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void GetEnemy::Initialize(){
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void GetEnemy::Start(){

}

//毎フレーム呼ばれます
void GetEnemy::Update(){
	
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void GetEnemy::Finish(){

}

//コライダーとのヒット時に呼ばれます
void GetEnemy::OnCollideBegin(GameObject target){
	if (!target)return;
	if (!Enemy::GetEnemy(target))return;
	if (Enemy::GetEnemy(target)->GetEnemyAllParameter(false).hp > 0.0f) {

		m_EnemyList.push_back(target);
	}
}

//コライダーとのヒット中に呼ばれます
void GetEnemy::OnCollideEnter(GameObject target){

}

//コライダーとのロスト時に呼ばれます
void GetEnemy::OnCollideExit(GameObject target){
	if (!target)return;
	if (!Enemy::GetEnemy(target))return;
	m_EnemyList.remove_if([&](auto o) {
		return o == target || !o; });
}

GameObject GetEnemy::GetMinEnemy()
{
//	auto pos = gameObject->mTransform->WorldPosition();
//	float lowL = 9999999.0f;
//	GameObject lowObj = NULL;
//	for (auto& enemy : m_EnemyList) {
//		if (!enemy) continue;
//		auto l = XMVector3Length(pos - enemy->mTransform->WorldPosition()).x;
//		if (l < lowL) {
//			lowObj = enemy;
//			lowL = l;
//		}
//	}
//	return lowObj;
	auto m_Camera = UniqueObject::GetCamera();
	if (!m_Camera)return NULL;
	XMVECTOR camvec;
	camvec = m_Camera->mTransform->Forward();
	
	float lowL = 9999999.0f;
	GameObject lowObj = NULL;

	for (auto& enemy : m_EnemyList) {
		if (!enemy) continue;

		auto pos = gameObject->mTransform->WorldPosition();
		auto epos = enemy->mTransform->WorldPosition();
		epos.y += 1.0f;
		auto v = epos - pos;
		if (Hx::PhysX()->Raycast(pos, XMVector3Normalize(v), XMVector3Length(v).x, Layer::UserTag4)) {
			continue;
		}


		XMVECTOR vect2 = XMVector3Normalize(enemy->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition());
		float dot = XMVector3Dot(camvec, vect2).x;
		if (dot < 0) {
			continue;
		}
		float l = XMVector3Length(v).x;
		dot = 1.0f - dot;
		dot += l/50.0f;
		if (dot < lowL) {
			lowObj = enemy;
			lowL = dot;
		}
	}
	return lowObj;
}

GameObject GetEnemy::GetPointMinEnemy(GameObject currentTarget, MinVect::Enum minVect)
{
	if (!currentTarget)return NULL;
	XMVECTOR camvec;
	auto m_Camera = UniqueObject::GetCamera();
	if (m_Camera) {
		camvec = m_Camera->mTransform->Forward();
	}
	XMVECTOR vect = XMVector3Normalize(currentTarget->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition());
	float lowL = 9999999.0f;
	GameObject lowObj = NULL;


	auto mat = XMMatrixLookToLH(XMVectorZero(), vect, XMVectorSet(0, 1, 0, 1));
	mat = XMMatrixTranspose(mat);
	XMVECTOR Null;
	mat = XMMatrixInverse(&Null, mat);

	auto temp = XMMatrixMultiply(XMMatrixTranslationFromVector(currentTarget->mTransform->WorldPosition()), mat);
	auto CurrentTargetPos = temp.r[3];

	for (auto& enemy : m_EnemyList) {
		if (!enemy) continue;
		auto pos = gameObject->mTransform->WorldPosition();
		auto epos = enemy->mTransform->WorldPosition();
		epos.y += 1.0f;
		auto v = epos - pos;
		if (Hx::PhysX()->Raycast(pos, XMVector3Normalize(v), XMVector3Length(v).x, Layer::UserTag4)) {
			continue;
		}


		auto posmat = XMMatrixMultiply(XMMatrixTranslationFromVector(enemy->mTransform->WorldPosition()), mat);
		float l = abs(posmat.r[3].x - CurrentTargetPos.x);

		XMVECTOR vect2 = XMVector3Normalize(enemy->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition());
		if (m_Camera) {
			if (XMVector3Dot(camvec, vect2).x < 0)continue;
		}

		if (minVect == MinVect::left) {

			if (posmat.r[3].x < CurrentTargetPos.x) {
				if (l < lowL) {
					lowObj = enemy;
					lowL = l;
				}
			}
		}
		else if (minVect == MinVect::right) {
			if (posmat.r[3].x > CurrentTargetPos.x) {
				if (l < lowL) {
					lowObj = enemy;
					lowL = l;
				}
			}
		}
	}
	return lowObj;
}

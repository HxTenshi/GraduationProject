#include "GetEnemy.h"
#include "h_standard.h"
#include "Enemy.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GetEnemy::Initialize(){
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GetEnemy::Start(){

}

//���t���[���Ă΂�܂�
void GetEnemy::Update(){
	
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void GetEnemy::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GetEnemy::OnCollideBegin(GameObject target){
	if (!target)return;
	if (!Enemy::GetEnemy(target))return;
	m_EnemyList.push_back(target);
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GetEnemy::OnCollideEnter(GameObject target){

}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void GetEnemy::OnCollideExit(GameObject target){
	if (!target)return;
	if (!Enemy::GetEnemy(target))return;
	m_EnemyList.remove_if([&](auto o) {
		return o == target || !o; });
}

GameObject GetEnemy::GetMinEnemy()
{
	auto pos = gameObject->mTransform->WorldPosition();
	float lowL = 9999999.0f;
	GameObject lowObj = NULL;
	for (auto& enemy : m_EnemyList) {
		if (!enemy) continue;
		auto l = XMVector3Length(pos - enemy->mTransform->WorldPosition()).x;
		if (l < lowL) {
			lowObj = enemy;
			lowL = l;
		}
	}
	return lowObj;
}

GameObject GetEnemy::GetPointMinEnemy(GameObject currentTarget, MinVect::Enum minVect)
{
	if (!currentTarget)return NULL;
	XMVECTOR camvec;
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

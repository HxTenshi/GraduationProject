#include "GetEnemy.h"
#include "h_standard.h"
#include "Sandbag.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GetEnemy::Initialize(){
	m_MinEnemy = NULL;
	m_MinEnemy_Back = NULL;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GetEnemy::Start(){

}

//���t���[���Ă΂�܂�
void GetEnemy::Update(){
	m_MinEnemy_Back = m_MinEnemy;
	m_MinEnemy = NULL;
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void GetEnemy::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GetEnemy::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
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

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void GetEnemy::OnCollideExit(GameObject target){
	(void)target;
}

GameObject GetEnemy::GetMinEnemy()
{
	return m_MinEnemy ? m_MinEnemy : m_MinEnemy_Back ? m_MinEnemy_Back : NULL;
}

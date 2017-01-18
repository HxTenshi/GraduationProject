#include "EnemyArrow.h"
#include "Enemy.h"
#include "../h_standard.h"
#include "../h_component.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyArrow::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyArrow::Start(){

}

//���t���[���Ă΂�܂�
void EnemyArrow::Update(){
	gameObject->mTransform->WorldPosition(gameObject->mTransform->WorldPosition() + m_Vec * speed * Hx::DeltaTime()->GetDeltaTime());
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void EnemyArrow::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyArrow::OnCollideBegin(GameObject target){
	if (!m_Enemy)return;
	auto enemyScript = Enemy::GetEnemy(m_Enemy);
	if (!enemyScript)return;
	enemyScript->Attack(target,COL_TYPE::NORMAL);
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyArrow::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void EnemyArrow::OnCollideExit(GameObject target){
	(void)target;
}
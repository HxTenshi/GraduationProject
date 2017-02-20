#include "EnemyArrow.h"
#include "Enemy.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "UniqueObject.h"
#include "PlayerController.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyArrow::Initialize(){
	count = 0;
	m_Stop = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyArrow::Start(){

}

//���t���[���Ă΂�܂�
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

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void EnemyArrow::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
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

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyArrow::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void EnemyArrow::OnCollideExit(GameObject target){
	(void)target;
}
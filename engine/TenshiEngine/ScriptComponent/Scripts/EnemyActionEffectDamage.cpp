#include "EnemyActionEffectDamage.h"
#include "PlayerController.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyActionEffectDamage::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyActionEffectDamage::Start(){
	m_hit = false;
}

//���t���[���Ă΂�܂�
void EnemyActionEffectDamage::Update(){
	m_timer += Hx::DeltaTime()->GetDeltaTime();
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void EnemyActionEffectDamage::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyActionEffectDamage::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
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

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void EnemyActionEffectDamage::OnCollideExit(GameObject target){
	(void)target;
}
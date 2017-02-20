#include "BombDamageEffect.h"
#include "h_standard.h"
#include "PlayerController.h"
#include "Enemy.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void BombDamageEffect::Initialize(){
	m_dead = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void BombDamageEffect::Start(){

}

//���t���[���Ă΂�܂�
void BombDamageEffect::Update(){
	if (m_dead) {
		Hx::DestroyObject(gameObject);
	}
	m_dead = true;
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void BombDamageEffect::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void BombDamageEffect::OnCollideBegin(GameObject target){
	if (target) {
		auto pos1 = target->mTransform->WorldPosition();
		auto pos2 = gameObject->mTransform->WorldPosition();
		auto vect = pos1 - pos2;
		vect.y = 0.0f;
		if (XMVector3Length(vect).x != 0.0f) {
			vect.z = 1.0f;
		}
		vect = XMVector3Normalize(vect) * m_KnockBackPower;
		float time = Hx::DeltaTime()->GetDeltaTime();
		auto player = target->GetScript<PlayerController>();
		if (player) {
			player->Damage(m_Damage, vect, PlayerController::KnockBack::Down, false, false);
		}

		auto enemy = Enemy::GetEnemy(target);
		if (enemy) {
			enemy->Damage(m_Damage, BATTLEACTION::WINCEACTION , vect);
		}
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void BombDamageEffect::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void BombDamageEffect::OnCollideExit(GameObject target){
	(void)target;
}

void BombDamageEffect::SetDamage(float value)
{
	m_Damage = value;
}

void BombDamageEffect::SetKnockBackPower(float value)
{
	m_KnockBackPower = value;
}

float BombDamageEffect::GetDamage()
{
	return m_Damage;
}

float BombDamageEffect::GetKnockBackPower()
{
	return m_KnockBackPower;
}

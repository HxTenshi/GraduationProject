#include "BombDamageEffect.h"
#include "h_standard.h"
#include "PlayerController.h"
#include "Enemy.h"
#include "SoundManager.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void BombDamageEffect::Initialize(){
	m_dead = false;
	m_PlaySE = true;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void BombDamageEffect::Start(){

}

//毎フレーム呼ばれます
void BombDamageEffect::Update(){
	if (m_PlaySE) {
		m_PlaySE = false;
		SoundManager::PlaySE(SoundManager::SoundSE_ID::Bomb2, gameObject->mTransform->WorldPosition());
	}
	if (m_dead) {
		Hx::DestroyObject(gameObject);
	}
	m_dead = true;
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void BombDamageEffect::Finish(){

}

//コライダーとのヒット時に呼ばれます
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
			enemy->Damage(m_Damage * 2, BATTLEACTION::WINCEACTION , vect);
		}
	}
}

//コライダーとのヒット中に呼ばれます
void BombDamageEffect::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
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

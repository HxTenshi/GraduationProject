#include "PlayerSpecialDamageArea.h"
#include "UniqueObject.h"
#include "PlayerController.h"
#include "h_standard.h"
#include "Enemy.h"
#include "Weapon.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PlayerSpecialDamageArea::Initialize(){
	m_Damage = 0.0f;
	m_Dead = false;

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PlayerSpecialDamageArea::Start(){

	if (auto p = UniqueObject::GetPlayer())
	{
		if(auto pc = p->GetScript<PlayerController>()){
			if (auto w = pc->GetWeapon()) {
				m_Damage = (w->GetAttackPower()*0.5+10) * 10.0f;
			}
		}
	}

}

//���t���[���Ă΂�܂�
void PlayerSpecialDamageArea::Update(){

	if (m_Dead) {
		Hx::DestroyObject(gameObject);
	}
	m_Dead = true;
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void PlayerSpecialDamageArea::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PlayerSpecialDamageArea::OnCollideBegin(GameObject target){
	if (target) {
		auto p = UniqueObject::GetPlayer();
		if (!p)return;
		auto pos1 = target->mTransform->WorldPosition();
		auto pos2 = p->mTransform->WorldPosition();
		auto vect = pos1 - pos2;
		vect.y = 0.0f;
		if (XMVector3Length(vect).x != 0.0f) {
			vect.z = 1.0f;
		}
		vect = XMVector3Normalize(vect);

		auto enemy = Enemy::GetEnemy(target);
		if (enemy) {
			enemy->Damage(m_Damage , BATTLEACTION::WINCEACTION, vect);
		}
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PlayerSpecialDamageArea::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void PlayerSpecialDamageArea::OnCollideExit(GameObject target){
	(void)target;
}
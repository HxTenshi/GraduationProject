#include "PlayerSpecialDamageArea.h"
#include "UniqueObject.h"
#include "PlayerController.h"
#include "h_standard.h"
#include "Enemy.h"
#include "Weapon.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void PlayerSpecialDamageArea::Initialize(){
	m_Damage = 0.0f;
	m_Dead = false;

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
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

//毎フレーム呼ばれます
void PlayerSpecialDamageArea::Update(){

	if (m_Dead) {
		Hx::DestroyObject(gameObject);
	}
	m_Dead = true;
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void PlayerSpecialDamageArea::Finish(){

}

//コライダーとのヒット時に呼ばれます
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

//コライダーとのヒット中に呼ばれます
void PlayerSpecialDamageArea::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void PlayerSpecialDamageArea::OnCollideExit(GameObject target){
	(void)target;
}
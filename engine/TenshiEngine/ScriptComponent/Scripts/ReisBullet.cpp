#include "ReisBullet.h"

#include "h_component.h"
#include "h_standard.h"
#include "UniqueObject.h"
#include "SoundManager.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void ReisBullet::Initialize(){
	m_IsDestroy=false;
	m_AliveTimer=0.0f;
	m_DestroyWaitTimer=0.0f;
	m_PlaySE_Shot = true;
	m_PlaySE_Spawn = true;

	m_Vector = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ReisBullet::Start(){
	if (m_PlayerTarget) {
		PlayerLockOn();
	}

	SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_SE_Burret_Spawn, gameObject->mTransform->WorldPosition());
}

//毎フレーム呼ばれます
void ReisBullet::Update(){
	if (m_PlaySE_Spawn) {
		m_PlaySE_Spawn = false;
		SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_SE_Burret_Shot, gameObject->mTransform->WorldPosition());
	}
	auto time = Hx::DeltaTime()->GetDeltaTime();
	if (m_IsDestroy) {
		if (m_DestroyWaitTimer == 0.0f) {
			if (auto obj = Hx::Instance(m_DestroyParticle)) {
				obj->mTransform->WorldPosition(gameObject->mTransform->WorldPosition());
			}

			if (auto col = gameObject->GetComponent<PhysXColliderComponent>()) {
				col->Disable();
			}
			if (m_BulletModel) {
				m_BulletModel->Disable();
			}
		}
		m_DestroyWaitTimer += time;
		if (m_DestroyWaitTime < m_DestroyWaitTimer) {
			Hx::DestroyObject(gameObject);
		}
	}
	else{
		m_AliveTimer += time;
		if (m_WaitTime < m_AliveTimer) {
			if (m_PlaySE_Shot) {
				m_PlaySE_Shot = false;
				SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_SE_Burret_Shot, gameObject->mTransform->WorldPosition());
			}
			auto pos = gameObject->mTransform->WorldPosition();
			pos += m_Vector * m_Speed * time;
			gameObject->mTransform->WorldPosition(pos);
		}
		else {
			if (m_PlayerTarget) {
				PlayerLockOn();
			}
		}

		if (m_AliveTimer >= 60.0f) {
			m_IsDestroy = true;
		}
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ReisBullet::Finish(){

}
#include "BurstDamageArea.h"
//コライダーとのヒット時に呼ばれます
void ReisBullet::OnCollideBegin(GameObject target){
	m_IsDestroy = true;

	if (m_BulletModel) {
		if (auto scr = m_BulletModel->GetScript<BurstDamageArea>()) {
			scr->OnCollideBegin(target);
		}
	}
	if (target->GetLayer() == 4) {

		SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_SE_Burret_Hit, gameObject->mTransform->WorldPosition());
	}
}

//コライダーとのヒット中に呼ばれます
void ReisBullet::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void ReisBullet::OnCollideExit(GameObject target){
	(void)target;
}

void ReisBullet::PlayerLockOn()
{
	if (auto player = UniqueObject::GetPlayer()) {
		auto p = player->mTransform->WorldPosition();
		p.y += 1.0f;
		auto v = p - gameObject->mTransform->WorldPosition();
		if (XMVector3Length(v).x != 0.0f) {
			m_Vector = XMVector3Normalize(v);
		}
	}
}

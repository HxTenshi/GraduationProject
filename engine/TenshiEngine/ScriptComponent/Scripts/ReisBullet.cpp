#include "ReisBullet.h"

#include "h_component.h"
#include "h_standard.h"
#include "UniqueObject.h"
#include "SoundManager.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ReisBullet::Initialize(){
	m_IsDestroy=false;
	m_AliveTimer=0.0f;
	m_DestroyWaitTimer=0.0f;
	m_PlaySE_Shot = true;
	m_PlaySE_Spawn = true;

	m_Vector = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ReisBullet::Start(){
	if (m_PlayerTarget) {
		PlayerLockOn();
	}

	SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_SE_Burret_Spawn, gameObject->mTransform->WorldPosition());
}

//���t���[���Ă΂�܂�
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

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ReisBullet::Finish(){

}
#include "BurstDamageArea.h"
//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
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

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ReisBullet::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
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
#include "WeaponEffectCtr.h"
#include "h_standard.h"
#include "Game/Component/ParticleComponent.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponEffectCtr::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponEffectCtr::Start(){

}

//���t���[���Ă΂�܂�
void WeaponEffectCtr::Update(){
	m_now_lifetime += Hx::DeltaTime()->GetDeltaTime();
	if (m_stop_particle_time < m_now_lifetime) {
		if (auto scr = gameObject->GetComponent<ParticleComponent>()) {
			scr->mParticleParam.Param.x = 0;
			gameObject->mTransform->SetParent(nullptr);
		}
	}
	if (m_lifetime < m_now_lifetime) {
		Hx::DestroyObject(gameObject);
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void WeaponEffectCtr::Finish(){

}
void WeaponEffectCtr::SetLifeTime(float time,float stoptime) {
	m_lifetime = time;
	m_stop_particle_time = stoptime;
}

void WeaponEffectCtr::Reset()
{
	m_now_lifetime = 0.0f;
}

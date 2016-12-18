#include "WeaponEffect.h"
#include"h_standard.h"
#include "WeaponEffectCtr.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponEffect::Initialize(){
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponEffect::Start(){

}

//���t���[���Ă΂�܂�
void WeaponEffect::Update(){
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void WeaponEffect::Finish(){

}

void WeaponEffect::Action()
{
	if (m_particle.IsLoad()) {
		m_now_Effect = Hx::Instance(m_particle);
		m_now_Effect->mTransform->SetParent(gameObject);
		m_now_Effect->mTransform->Position(XMVectorSet(0, 0, 0, 0));
		if (auto scr = m_now_Effect->GetScript<WeaponEffectCtr>()) {
			scr->SetLifeTime(m_life_time, m_stop_particle_time);
		}
	}
}

void WeaponEffect::SetLifeTime(float time,float dtime)
{
	m_stop_particle_time = time;
	m_life_time = time + dtime;
}

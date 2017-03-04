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

void WeaponEffect::Action(WeaponEffectType type)
{
	Hx::Debug()->Log("������");
	if (m_particle.IsLoad()&&m_throw_particle.IsLoad()) {
		auto p = (type == WeaponEffectType::STRONGEF) ? Hx::Instance(m_particle) : Hx::Instance(m_throw_particle);
		m_nowParticle = p;
		auto point = (type == WeaponEffectType::STRONGEF) ? gameObject : weaponTip;
		Hx::Debug()->Log("������");
		if (!point)point=gameObject;
		p->mTransform->SetParent(point);
		p->mTransform->Position(XMVectorSet(0, 0, 0, 0));
		if (auto scr = p->GetScript<WeaponEffectCtr>()) {
			scr->SetLifeTime(m_life_time, m_stop_particle_time);
			Hx::Debug()->Log(std::to_string(type));
		}
	}
}
void WeaponEffect::DeleteParticle(float time, float dtime)
{
	if (m_nowParticle) {
		if (auto scr = m_nowParticle->GetScript<WeaponEffectCtr>()) {
			scr->Reset();
			scr->SetLifeTime(time,time+dtime);
		}
	}
}
//�X�g�b�v�^�C���𒴂����Capacity���[���ɂȂ�
void WeaponEffect::SetLifeTime(float time,float dtime)
{
	m_stop_particle_time = time;
	m_life_time = time + dtime;
}

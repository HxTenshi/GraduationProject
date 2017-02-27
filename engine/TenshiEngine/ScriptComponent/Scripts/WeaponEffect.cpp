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
	
	if (m_particle.IsLoad()&&m_throw_particle.IsLoad()) {
		auto p = (type == WeaponEffectType::STRONGEF) ? Hx::Instance(m_particle) : Hx::Instance(m_throw_particle);
		auto point = (type == WeaponEffectType::STRONGEF) ? gameObject : weaponTip;
		if (!point)point=gameObject;
		p->mTransform->SetParent(point);
		p->mTransform->Position(XMVectorSet(0, 0, 0, 0));
		if (auto scr = p->GetScript<WeaponEffectCtr>()) {
			scr->SetLifeTime(m_life_time, m_stop_particle_time);
		}
	}
}
//�X�g�b�v�^�C���𒴂����Capacity���[���ɂȂ�
void WeaponEffect::SetLifeTime(float time,float dtime)
{
	m_stop_particle_time = time;
	m_life_time = time + dtime;
}

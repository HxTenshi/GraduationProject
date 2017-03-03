#include "WeaponEffect.h"
#include"h_standard.h"
#include "WeaponEffectCtr.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void WeaponEffect::Initialize(){
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WeaponEffect::Start(){

}

//毎フレーム呼ばれます
void WeaponEffect::Update(){
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void WeaponEffect::Finish(){

}

void WeaponEffect::Action(WeaponEffectType type)
{
	Hx::Debug()->Log("投げる");
	if (m_particle.IsLoad()&&m_throw_particle.IsLoad()) {
		auto p = (type == WeaponEffectType::STRONGEF) ? Hx::Instance(m_particle) : Hx::Instance(m_throw_particle);
		m_nowParticle = p;
		auto point = (type == WeaponEffectType::STRONGEF) ? gameObject : weaponTip;
		Hx::Debug()->Log("投げる");
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
//ストップタイムを超えるとCapacityがゼロになる
void WeaponEffect::SetLifeTime(float time,float dtime)
{
	m_stop_particle_time = time;
	m_life_time = time + dtime;
}

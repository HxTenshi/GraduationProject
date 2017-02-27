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
//ストップタイムを超えるとCapacityがゼロになる
void WeaponEffect::SetLifeTime(float time,float dtime)
{
	m_stop_particle_time = time;
	m_life_time = time + dtime;
}

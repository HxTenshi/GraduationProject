#include "WeaponEffectCtr.h"
#include "h_standard.h"
#include "Game/Component/ParticleComponent.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void WeaponEffectCtr::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WeaponEffectCtr::Start(){

}

//毎フレーム呼ばれます
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

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
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

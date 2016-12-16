#include "WeaponEffect.h"
#include"h_standard.h"
//生成時に呼ばれます（エディター中も呼ばれます）
void WeaponEffect::Initialize(){
	m_now_life_time = 0;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WeaponEffect::Start(){

}

//毎フレーム呼ばれます
void WeaponEffect::Update(){
	DeleteAction();
	Play();
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void WeaponEffect::Finish(){

}

void WeaponEffect::Action()
{
	if (m_now_Effect)Reset();
	if (m_particle.IsLoad()&(!m_now_Effect)) {
		m_now_Effect = Hx::Instance(m_particle);
		m_now_Effect->mTransform->SetParent(gameObject);
		m_now_Effect->mTransform->Position(XMVectorSet(0, 0, 0, 0));
	}
}

void WeaponEffect::Play()
{
	if(m_now_Effect)m_now_life_time += Hx::DeltaTime()->GetDeltaTime();
}

void WeaponEffect::Reset()
{
	m_now_life_time = 0;
}

bool WeaponEffect::isAlive()
{
	return (m_life_time>m_now_life_time);
}

void WeaponEffect::DeleteAction()
{
	if (m_life_time < m_now_life_time) {
		Hx::DestroyObject(m_now_Effect);
		Reset();
	}
}

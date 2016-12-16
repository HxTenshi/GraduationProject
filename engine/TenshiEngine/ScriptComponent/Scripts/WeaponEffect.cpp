#include "WeaponEffect.h"
#include"h_standard.h"
//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponEffect::Initialize(){
	m_now_life_time = 0;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponEffect::Start(){

}

//���t���[���Ă΂�܂�
void WeaponEffect::Update(){
	DeleteAction();
	Play();
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
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

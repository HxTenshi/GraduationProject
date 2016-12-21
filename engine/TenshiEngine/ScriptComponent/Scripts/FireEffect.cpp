#include "FireEffect.h"
#include "h_component.h"
#include "h_standard.h"
#include "Library/easing.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void FireEffect::Initialize(){
	m_Timer = 0.0f;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void FireEffect::Start(){

}

//���t���[���Ă΂�܂�
void FireEffect::Update(){

	auto com = gameObject->GetComponent<PointLightComponent>();
	if (com) {
		float t = 0;
		m_Timer += rand()/(float)RAND_MAX * m_RandomPower * Hx::DeltaTime()->GetDeltaTime();
		t = std::sin(m_Timer)/2.0f+0.5f;
		com->m_HDR = (float)Easing::Linear((double)t,1.0, (double)m_MaxHDR, (double)m_MinHDR);
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void FireEffect::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void FireEffect::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void FireEffect::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void FireEffect::OnCollideExit(GameObject target){
	(void)target;
}
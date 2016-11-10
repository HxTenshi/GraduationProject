#include "InputClockGimick.h"
#include "h_standard.h"
#include "OutputGimic.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void InputClockGimick::Initialize(){
	m_TimeCounter = 0.0f;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void InputClockGimick::Start(){

}

//���t���[���Ă΂�܂�
void InputClockGimick::Update(){
	if (m_Timer <= 0.0f)return;
	if (!m_Output)return;

	auto scr = OutputGimic::GetOutputGimic(m_Output);
	if (!scr)return;

	m_TimeCounter += Hx::DeltaTime()->GetDeltaTime();
	for(int i= 0;i<12;i++){
		if(m_TimeCounter >= m_Timer){
			scr->OnStart(gameObject);
			m_TimeCounter -= m_Timer;
		}
		else {
			break;
		}
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void InputClockGimick::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void InputClockGimick::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void InputClockGimick::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void InputClockGimick::OnCollideExit(GameObject target){
	(void)target;
}
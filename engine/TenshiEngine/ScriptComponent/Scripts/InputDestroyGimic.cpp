#include "InputDestroyGimic.h"
#include "OutputGimic.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void InputDestroyGimic::Initialize(){
	m_End = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void InputDestroyGimic::Start(){

}

//���t���[���Ă΂�܂�
void InputDestroyGimic::Update(){

	if (m_End)return;
	if (!m_OutputGimic)return;
	if (m_Target)return;

	auto scr = OutputGimic::GetOutputGimic(m_OutputGimic);
	if (!scr)return;

	scr->OnStart(gameObject);
	m_End = true;
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void InputDestroyGimic::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void InputDestroyGimic::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void InputDestroyGimic::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void InputDestroyGimic::OnCollideExit(GameObject target){
	(void)target;
}
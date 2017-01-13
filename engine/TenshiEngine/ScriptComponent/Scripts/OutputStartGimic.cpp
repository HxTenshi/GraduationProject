#include "OutputStartGimic.h"
#include "Matinee.h"
#include "h_standard.h"
//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void OutputStartGimic::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void OutputStartGimic::Start(){

}

//���t���[���Ă΂�܂�
void OutputStartGimic::Update(){

	if (Input::Trigger(KeyCode::Key_L)) {
		OnStart(NULL);
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void OutputStartGimic::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void OutputStartGimic::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void OutputStartGimic::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void OutputStartGimic::OnCollideExit(GameObject target){
	(void)target;
}

bool OutputStartGimic::OnStart(GameObject Sender){
	if (!m_Output)return false;

	if (m_StartMatineeMode) {
		auto scr = m_Output->GetScript<Matinee>();
		if (!scr)return false;

		scr->Play();

		return scr->IsPlay();
	}

	if (m_StartInstanceObjectMode) {
		auto ins = Hx::Instance(m_Output);
		ins->Enable();
		return (bool)ins;
	}
}
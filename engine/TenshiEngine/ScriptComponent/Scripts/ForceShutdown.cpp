#include "ForceShutdown.h"

#include "h_standard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ForceShutdown::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ForceShutdown::Start(){

}

//���t���[���Ă΂�܂�
void ForceShutdown::Update(){

	if (Input::Down(PAD_X_KeyCode::Button_START) && Input::Down(PAD_X_KeyCode::Button_BACK)) {
		Hx::Shutdown();
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ForceShutdown::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ForceShutdown::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ForceShutdown::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void ForceShutdown::OnCollideExit(GameObject target){
	(void)target;
}
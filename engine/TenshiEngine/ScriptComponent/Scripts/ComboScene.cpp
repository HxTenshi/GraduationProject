#include "ComboScene.h"
#include "SoundManager.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ComboScene::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ComboScene::Start(){

}

//���t���[���Ă΂�܂�
void ComboScene::Update(){
	if (Input::Trigger(KeyCode::Key_SPACE)) {
		Hx::LoadScene(configPass);
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ComboScene::Finish(){
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ComboScene::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ComboScene::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void ComboScene::OnCollideExit(GameObject target){
	(void)target;
}
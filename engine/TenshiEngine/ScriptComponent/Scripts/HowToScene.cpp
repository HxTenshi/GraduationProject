#include "HowToScene.h"



//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void HowToScene::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void HowToScene::Start(){

}

//���t���[���Ă΂�܂�
void HowToScene::Update(){
	if (Input::Trigger(KeyCode::Key_SPACE)) {
		Hx::LoadScene(configPass);
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void HowToScene::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void HowToScene::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void HowToScene::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void HowToScene::OnCollideExit(GameObject target){
	(void)target;
}
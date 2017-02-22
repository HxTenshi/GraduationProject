#include "ComboScene.h"
#include "SoundManager.h"
#include "Fader.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ComboScene::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ComboScene::Start(){

}

//���t���[���Ă΂�܂�
void ComboScene::Update(){
	bool isSpaceKey = Input::Trigger(KeyCode::Key_SPACE);
	bool isPad_B_Button = Input::Trigger(PAD_X_KeyCode::Button_B);
	if (isSpaceKey || isPad_B_Button) {
		SoundManager::PlaySE(SoundManager::SoundSE_ID::Enum::Decision,XMVectorZero());
		if (!m_fader) return;
		auto fader = m_fader->GetScript<Fader>();
		fader->OnSceneChnage(configPass);
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
#include "HowToScene.h"
#include "Fader.h"
#include "SoundManager.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void HowToScene::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void HowToScene::Start(){

}

//���t���[���Ă΂�܂�
void HowToScene::Update(){
	if (m_fader->GetScript<Fader>()->IsFader())return;
	bool isSpaceKey = Input::Trigger(KeyCode::Key_SPACE);
	bool isPad_B_Button = Input::Trigger(PAD_X_KeyCode::Button_B);
	if (isSpaceKey || isPad_B_Button) {
		SoundManager::PlaySE_2D(SoundManager::SoundSE_ID::Enum::Decision);
		if (!m_fader) return;
		auto fader = m_fader->GetScript<Fader>();
		fader->OnSceneChnage(configPass);
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
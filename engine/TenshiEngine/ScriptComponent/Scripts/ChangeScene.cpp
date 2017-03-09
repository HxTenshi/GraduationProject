#include "ChangeScene.h"
#include "h_standard.h"
#include "Fader.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ChangeScene::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ChangeScene::Start(){

}

//���t���[���Ă΂�܂�
void ChangeScene::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ChangeScene::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ChangeScene::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ChangeScene::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void ChangeScene::OnCollideExit(GameObject target){
	(void)target;
}

void ChangeScene::Next()
{
	Hx::System()->LockCursorPositionToWindowCenter(false);
	if (!m_Fade)return;
	auto fade = m_Fade->GetScript<Fader>();
	if (!fade)return;

	fade->OnSceneChnage(m_SceneName);
	//Hx::LoadScene(m_SceneName);
}

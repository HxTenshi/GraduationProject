#include "GameOverManager.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GameOverManager::Initialize(){
	m_time = 0;
	m_telop->GetComponent<MaterialComponent>()->SetAlbedoColor(XMFLOAT4(1, 1, 1, 0));

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GameOverManager::Start(){
	
	
}

//���t���[���Ă΂�܂�
void GameOverManager::Update(){
	//m_material.SetAlbedoColor(XMFLOAT4(1, 1, 1, m_time*2.0f));
	ViewTelop();
	m_telop->GetComponent<MaterialComponent>()->SetAlbedoColor(XMFLOAT4(1, 1, 1, m_time*2.0f));
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void GameOverManager::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GameOverManager::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GameOverManager::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void GameOverManager::OnCollideExit(GameObject target){
	(void)target;
}

void GameOverManager::ViewTelop()
{
	m_time += Hx::DeltaTime()->GetDeltaTime()*0.01f;
	if (m_time > 0.5f)m_time = 0.5f;

}

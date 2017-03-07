#include "ChangeTexture.h"
#include "../h_standard.h"
#include "../h_component.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ChangeTexture::Initialize(){
	m_Count = 0.0f;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ChangeTexture::Start(){
	m_NowNumber = 0;
	gameObject->GetComponent<MaterialComponent>()->GetMaterialPtr(0)->SetTexture(m_Textures[m_NowNumber]);
}

//���t���[���Ă΂�܂�
void ChangeTexture::Update(){
	m_Count += Hx::DeltaTime()->GetDeltaTime();
	if (m_Count > m_ChangeTime) {
		m_Count = 0;
		m_NowNumber = (m_NowNumber == 0 ? 1 : 0);
		gameObject->GetComponent<MaterialComponent>()->GetMaterialPtr(0)->SetTexture(m_Textures[m_NowNumber]);
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ChangeTexture::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ChangeTexture::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ChangeTexture::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void ChangeTexture::OnCollideExit(GameObject target){
	(void)target;
}
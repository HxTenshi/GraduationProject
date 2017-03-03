#include "TextureFade.h"
#include "../h_standard.h"
#include "../h_component.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TextureFade::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TextureFade::Start(){
	auto color = gameObject->GetComponent<MaterialComponent>()->GetMaterialPtr(0)->GetAlbedo();
	color.w = m_Fade;
	gameObject->GetComponent<MaterialComponent>()->GetMaterialPtr(0)->SetAlbedo(color);
}

//���t���[���Ă΂�܂�
void TextureFade::Update(){
	m_StartTime += Hx::DeltaTime()->GetDeltaTime();
	if(m_StartTime >= 2.0f)
	m_Fade -= m_FadeSpeed * Hx::DeltaTime()->GetDeltaTime();
	if (m_Fade <= 0.0f)Hx::DestroyObject(gameObject);
	auto color = gameObject->GetComponent<MaterialComponent>()->GetMaterialPtr(0)->GetAlbedo();
	color.w = m_Fade;
	gameObject->GetComponent<MaterialComponent>()->GetMaterialPtr(0)->SetAlbedo(color);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void TextureFade::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void TextureFade::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void TextureFade::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void TextureFade::OnCollideExit(GameObject target){
	(void)target;
}
#include "TextureScroll.h"

#include "h_component.h"
#include "h_standard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TextureScroll::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TextureScroll::Start(){

}

//���t���[���Ă΂�܂�
void TextureScroll::Update(){

	auto mate = gameObject->GetComponent<MaterialComponent>();
	if (!mate)return;
	auto m = mate->GetMaterialPtr(0);
	if (!m)return;
	m->mOffset.x += m_Speed.x * Hx::DeltaTime()->GetDeltaTime();
	m->mOffset.y += m_Speed.y * Hx::DeltaTime()->GetDeltaTime();
	m->ParamUpdate();

	//auto m = mate->GetMaterial(0);
	//mate->SetMaterial(0,m);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void TextureScroll::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void TextureScroll::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void TextureScroll::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void TextureScroll::OnCollideExit(GameObject target){
	(void)target;
}
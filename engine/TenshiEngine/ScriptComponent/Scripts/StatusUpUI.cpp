#include "StatusUpUI.h"
#include "h_standard.h"
#include "h_component.h"
#include "Library\easing.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void StatusUpUI::Initialize(){
	m_UpTimer = 0.0f;
	m_AlphaTimer = 0.0f;
	m_SetY=0.0f;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void StatusUpUI::Start(){
}

//���t���[���Ă΂�܂�
void StatusUpUI::Update(){
	if (m_UpTimer == 0.0f) {
		m_SetY = gameObject->mTransform->Position().y;
	}
	auto time = Hx::DeltaTime()->GetDeltaTime();
	if (m_UpTime != 0.0f && m_UpTime >= m_UpTimer) {
		m_UpTimer += time;
		float t = m_UpTimer / m_UpTime;
		t = min(t, 1.0f);
		float y = Easing::OutExp(t, 1.0f, m_SetY + m_UpY, m_SetY);
		auto p = gameObject->mTransform->Position();
		p.y = y;
		gameObject->mTransform->Position(p);
	}else if (m_AlphaTime != 0.0f && m_AlphaTime >= m_AlphaTimer) {
		m_AlphaTimer += time;
		float t = m_AlphaTimer / m_AlphaTime;
		t = min(t, 1.0f);
		if (auto mat = gameObject->GetComponent<MaterialComponent>()) {
			auto al = mat->GetMaterialPtr(0)->GetAlbedo();
			al.w = 1.0f - t;
			mat->GetMaterialPtr(0)->SetAlbedo(al);
		}
	}
	else {
		Hx::DestroyObject(gameObject);
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void StatusUpUI::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void StatusUpUI::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void StatusUpUI::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void StatusUpUI::OnCollideExit(GameObject target){
	(void)target;
}
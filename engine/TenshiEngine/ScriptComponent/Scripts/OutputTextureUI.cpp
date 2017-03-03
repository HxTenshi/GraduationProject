#include "OutputTextureUI.h"
# include "h_standard.h"
#include "OutputGimic.h"
# include "Game\Component\MaterialComponent.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void OutputTextureUI::Initialize(){
	m_Timer = 0;
	m_FirstFrame = true;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void OutputTextureUI::Start(){

}

//���t���[���Ă΂�܂�
void OutputTextureUI::Update(){
	if (m_FirstFrame) {

	}
	m_FirstFrame = false;
	m_Timer += Hx::DeltaTime()->GetDeltaTime();
	if (m_Timer >= m_WaitTime) {
		auto mc = gameObject->GetComponent<MaterialComponent>();
		if (!mc)return;
		auto color = mc->mAlbedo;
		color.w -= m_TimeScale * Hx::DeltaTime()->GetDeltaTime();
		bool end = false;
		if (color.w <= 0.0f) {
			color.w = 0.0f;
			end = true;
		}
		mc->SetAlbedoColor(color);
		if (end) {
			if (!m_EndAction)return;
			auto sender = OutputGimic::GetOutputGimic(m_EndAction);
			if (sender)sender->OnStart(gameObject);
			Hx::DestroyObject(gameObject);
		}
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void OutputTextureUI::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void OutputTextureUI::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void OutputTextureUI::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void OutputTextureUI::OnCollideExit(GameObject target){
	(void)target;
}
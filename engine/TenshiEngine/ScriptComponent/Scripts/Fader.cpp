#include "Fader.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Fader::Initialize(){
	if (!m_faderTexObj) return;
	m_isFadeIn = false;
	m_isSceneChange = false;
	m_nextScenePass = "";

	m_faderTexObj->Enable();
	auto material = m_faderTexObj->GetComponent<MaterialComponent>();
	if (!material) return;

	material->GetMaterialPtr(0)->SetTexture(blackTex);
	material->GetMaterialPtr(0)->SetAlbedo(XMFLOAT4(1,1,1,1));
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Fader::Start(){

}

//���t���[���Ă΂�܂�
void Fader::Update(){
	if (!m_faderTexObj) return;
	//�}�e���A�����擾
	auto material = m_faderTexObj->GetComponent<MaterialComponent>();
	if (!material) return;
	//�}�e���A���̐F���擾
	XMFLOAT4 color = material->GetMaterialPtr(0)->GetAlbedo();

	//�t���O�ɉ����ĉ��Z���Z
	if (!m_isFadeIn) color.w -= 1.0f * Hx::DeltaTime()->GetDeltaTime();
	else color.w += 1.0f * Hx::DeltaTime()->GetDeltaTime(); 

	color.w = max(min(color.w, 1.0f), 0.0f);
	material->GetMaterialPtr(0)->SetAlbedo(color);

	

	if (m_isSceneChange && color.w >= 1.0f) {
		//�J�ڑO�Ƀe�N�X�`����ύX
		material->GetMaterialPtr(0)->SetTexture(loadingTex);
		
		Hx::Debug()->Log(m_nextScenePass + "�ɑJ�ڂ��܂���");
		Hx::LoadScene(m_nextScenePass);
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Fader::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Fader::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Fader::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Fader::OnCollideExit(GameObject target){
	(void)target;
}

void Fader::OnSceneChnage(std::string nextScenePass){
	if (m_isSceneChange) return;

	m_nextScenePass = nextScenePass;
	m_isFadeIn = true;
	m_isSceneChange = true;
}

#include "PauseMenuManager.h"
#include <h_standard.h>
#include <h_component.h>

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PauseMenuManager::Initialize(){
	m_state = State::Close;
	m_isItimie = false;
	m_num = 0;
	m_lerpTimers[0] = 0.0f;
	m_lerpTimers[1] = 0.0f;
	m_lerpTimers[2] = 0.0f;
	m_objMap[0].push_back(new Struct(m_texPause,  XMVectorSet(200,900,0,0)));
	m_objMap[1].push_back(new Struct(m_texHowTo,  XMVectorSet(960,740,0,0)));
	m_objMap[1].push_back(new Struct(m_texCombo,  XMVectorSet(960,540,0,0)));
	m_objMap[1].push_back(new Struct(m_texToTitle,XMVectorSet(960,340,0,0)));
	m_objMap[1].push_back(new Struct(m_texMigisita,XMVectorSet(1680,140,0,0)));
	m_objMap[2].push_back(new Struct(m_texItimie, XMVectorSet(1680,540,0,0)));
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PauseMenuManager::Start(){

}

//���t���[���Ă΂�܂�
void PauseMenuManager::Update(){
	if (Input::Trigger(KeyCode::Key_M)) {
		OpenPauseMenu();
	}
	if (m_state == State::Open) UpdateOpne();
	else if (m_state == State::Close) UpdateClose();
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void PauseMenuManager::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PauseMenuManager::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PauseMenuManager::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void PauseMenuManager::OnCollideExit(GameObject target){
	(void)target;
}

//�J���Ă���Ԃ̏���
void PauseMenuManager::UpdateOpne() {

	//Lerp����
	for (int i = 0; i < m_objMap.size(); i++) {
		//����̃e�N�X�`���̂ݏ����Œ�
		for (int j = 0; j < m_objMap[i].size(); j++) {
			if (m_objMap[i][j] == nullptr) return;
		}
	}

	//����̃|�[�Y�e�N�X�`��
	m_lerpTimers[0] += m_lerpSpeed * Hx::DeltaTime()->GetDeltaTime();
	LerpFunc(m_objMap[0][0], m_lerpTimers[0]);

	if (!m_isItimie) {
		//lerp�̌v�Z
		m_lerpTimers[1] += m_lerpSpeed * Hx::DeltaTime()->GetDeltaTime();
		m_lerpTimers[2] -= m_lerpSpeed * Hx::DeltaTime()->GetDeltaTime();
		//0�`3�̊ԂɃN�����v
		if (Input::Trigger(KeyCode::Key_UP)) m_num--;
		if (Input::Trigger(KeyCode::Key_DOWN)) m_num++;
		m_num = min(max(0, m_num), 3);

		//���ꂼ��̔������N����
		if (Input::Trigger(KeyCode::Key_SPACE)) MenuReAction(m_num);

		//�͂��L��,���W�ړ�
		m_kakoi->Enable();
		XMVECTOR pos = m_objMap[1][m_num]->m_texObj->mTransform->Position();
		m_kakoi->mTransform->Position(pos);
	}
	else {
		//lerp�̌v�Z
		m_lerpTimers[1] -= m_lerpSpeed * Hx::DeltaTime()->GetDeltaTime();
		m_lerpTimers[2] += m_lerpSpeed * Hx::DeltaTime()->GetDeltaTime();

		//�߂ɂ�[�����
		if (Input::Trigger(KeyCode::Key_SPACE))MenuReAction(4);

		//�͂�����
		m_kakoi->Disable();
	}

	//Lerp����
	for (int i = 0; i < m_objMap.size(); i++) {
		//����̃e�N�X�`���̂ݏ����Œ�
		for (int j = 0; j < m_objMap[i].size(); j++) {
			LerpFunc(m_objMap[i][j], m_lerpTimers[i]);
		}
	}
	//�E���̃e�N�X�`��
	LerpFunc(m_objMap[1][3], 1.0f);


	//�t�F�[�h�C��
	auto material = m_texBlack->GetComponent<MaterialComponent>();
	XMFLOAT4 color = material->GetMaterialPtr(0)->GetAlbedo();
	color.w += m_lerpSpeed * Hx::DeltaTime()->GetDeltaTime();
	if (color.w >= 0.8f) color.w = 0.8f;
	material->SetAlbedoColor(color);
}

//���Ă���Ԃ̏���
void PauseMenuManager::UpdateClose(){
	m_kakoi->Disable();
	for (int i = 0; i < m_objMap.size(); i++) {
		for (int j = 0; j < m_objMap[i].size(); j++) {
			//���ɃX���C�h�ړ�
			XMVECTOR tempPos = m_objMap[i][j]->m_texObj->mTransform->Position();
			tempPos.x -= 100.0f * Hx::DeltaTime()->GetDeltaTime();
			//x���W��0�������Ȃ��悤�ɏ���
			if (m_objMap[i][j]->m_texObj->mTransform->Position().x < 0.0f) {
				tempPos = m_objMap[i][j]->m_texObj->mTransform->Position();
				tempPos.x = 0.0f;
			}
			m_objMap[i][j]->m_texObj->mTransform->Position(tempPos);
			//�}�e���A���̃J���[�@�A���t�@�[�l���Z
			auto material = m_objMap[i][j]->m_texObj->GetComponent<MaterialComponent>();
			XMFLOAT4 tempColor = material->GetMaterialPtr(0)->GetAlbedo();
			tempColor.w -= m_lerpSpeed * Hx::DeltaTime()->GetDeltaTime();
			material->SetAlbedoColor(tempColor);
		}
	}

	//�t�F�[�h�A�E�g
	auto material = m_texBlack->GetComponent<MaterialComponent>();
	XMFLOAT4 color = material->GetMaterialPtr(0)->GetAlbedo();
	color.w -= m_lerpSpeed * Hx::DeltaTime()->GetDeltaTime();
	if (color.w <= 0.0f) color.w = 0.0f;
	material->SetAlbedoColor(color);
}

//�|�[�Y���j���[���J�����ۂ̏���
void PauseMenuManager::OpenPauseMenu(){
	m_state = State::Open;
	m_isItimie = false;
	m_num = 0;
	for (int i = 0; i < m_objMap.size(); i++) {
		m_lerpTimers[i] = 0.0f;
		for (int j = 0; j < m_objMap[i].size(); j++) {
			//�������W�ƈړ���̍��W���Z�b�g
			m_objMap[i][j]->m_pos1 = m_objMap[i][j]->m_pivotPos + XMVectorSet(-600, 0, 0, 0);
			m_objMap[i][j]->m_pos2 = m_objMap[i][j]->m_pivotPos;
			if (i == 2) {
				//�ꖇ�G�̏������W�ƈړ���̍��W���Z�b�g
				m_objMap[i][j]->m_pos1 = m_objMap[i][j]->m_pivotPos;
				m_objMap[i][j]->m_pos2 = m_objMap[i][j]->m_pivotPos + XMVectorSet(-600, 0, 0, 0);
			}
			//�}�e���A���̃J���[�@�A���t�@�[�l���Z
			auto material = m_objMap[i][j]->m_texObj->GetComponent<MaterialComponent>();
			material->SetAlbedoColor(XMFLOAT4(1, 1, 1, 0));
		}
	}
	auto material = m_texBlack->GetComponent<MaterialComponent>();
	material->GetMaterialPtr(0)->SetAlbedo(XMFLOAT4(1, 1, 1, 0));
	m_texBlack->Enable();
}

//�|�[�Y���j���[�����
void PauseMenuManager::ClosePauseMenu(){
	m_state = State::Close;
}

//���ꂼ��̏������Ă�
void PauseMenuManager::MenuReAction(int num){

	switch (num)
	{
	case 0:	ChangeItimaie(Itimie::HowTo);	//������@�ɐ؂�ւ���
		break;
	case 1:	ChangeItimaie(Itimie::Combo);	//�R���{�\�ɐ؂�ւ���
		break;
	case 2:	BackToTitle();		//�^�C�g���ɔ��
		break;
	case 3: ClosePauseMenu();	//���j���[�����
		break;
	case 4: BackMenu();			//1���G���烁�j���[�ɖ߂�
		break;
	default:
		break;
	}
}

//�Ή������ꖇ�G�̐؂�ւ���
void PauseMenuManager::ChangeItimaie(Itimie type){
	if (m_objMap[2][0] == nullptr) return;
	if (m_objMap[1][3] == nullptr) return;

	m_isItimie = true;
	TextureAsset texItimaie;
	switch (type)
	{
	case PauseMenuManager::HowTo: texItimaie = texHowTo;
		break;
	case PauseMenuManager::Combo: texItimaie = texCombo;
		break;
	}
	auto material1 = m_objMap[2][0]->m_texObj->GetComponent<MaterialComponent>();
	material1->GetMaterialPtr(0)->SetTexture(texItimaie);
	auto material2 = m_objMap[1][3]->m_texObj->GetComponent<MaterialComponent>();
	material2->GetMaterialPtr(0)->SetTexture(texBack);

}

//1���G���烁�j���[���X�g�ɖ߂�
void PauseMenuManager::BackMenu(){
	if (m_objMap[1][3] == nullptr) return;
	m_isItimie = false;
	auto material = m_objMap[1][3]->m_texObj->GetComponent<MaterialComponent>();
	material->GetMaterialPtr(0)->SetTexture(texClose);
}

//�^�C�g���ɔ��
void PauseMenuManager::BackToTitle(){
	Hx::LoadScene("Assets/TitleScene.scene");
}

//Lerp��p��������
void PauseMenuManager::LerpFunc(Struct* structObj, float lerpTime){
	if (structObj == nullptr) return;
	//�N�����v����
	lerpTime = min(max(0.0f, lerpTime), 1.0f);
	//���W�ړ�
	XMVECTOR tempPos = XMVectorLerp(structObj->m_pos1, structObj->m_pos2, lerpTime);
	structObj->m_texObj->mTransform->Position(tempPos);
	//�}�e���A���̃A���t�@�l�̌v�Z
	auto material = structObj->m_texObj->GetComponent<MaterialComponent>();
	material->SetAlbedoColor(XMFLOAT4(1, 1, 1, lerpTime));
}

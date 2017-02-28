#include "PauseMenuManager.h"
#include <h_standard.h>
#include <h_component.h>

#include "UniqueObject.h"
#include "PlayerController.h"
#include "Matinee.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PauseMenuManager::Initialize(){
	m_state = State::Close;
	m_isItimie = false;
	m_num = 0;
	m_stickInterval = 0.0f;
	m_lerpTimers[0] = 0.0f;
	m_lerpTimers[1] = 0.0f;
	m_lerpTimers[2] = 0.0f;
	m_objMap[0].push_back(new Struct(m_texPause,  XMVectorSet(200,900,10001,0)));
	m_objMap[1].push_back(new Struct(m_texHowTo,  XMVectorSet(960,740,10001,0)));
	m_objMap[1].push_back(new Struct(m_texCombo,  XMVectorSet(960,540,10001,0)));
	m_objMap[1].push_back(new Struct(m_texToTitle,XMVectorSet(960,340,10001,0)));
	m_objMap[1].push_back(new Struct(m_texMigisita,XMVectorSet(1680,140,10001,0)));
	m_objMap[2].push_back(new Struct(m_texItimie, XMVectorSet(1680,540,10001,0)));
	this->gameObject->Enable();
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PauseMenuManager::Start(){

}

//���t���[���Ă΂�܂�
void PauseMenuManager::Update() {
	bool isStartKey = Input::Trigger(PAD_X_KeyCode::Button_START);
	if (Input::Trigger(KeyCode::Key_M) || isStartKey) {
		if(m_state == State::Close) OpenPauseMenu();
		else if (m_state == State::Open) ClosePauseMenu();
	}
	if (m_state == State::Open) { 
		Hx::DeltaTime()->SetTimeScale(0.0f);
		UpdateOpne(); 
	}
	else if (m_state == State::Close) { 
		UpdateClose(); 
	}
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

	//���^�[������
	for (int i = 0; i < m_objMap.size(); i++)
		for (int j = 0; j < m_objMap[i].size(); j++)
			if (m_objMap[i][j] == nullptr) return;

	//���͊Ԋu
	const float STICK_INTERVAL = 0.15f;
	//�C���^�[�o�����Z
	m_stickInterval += 1.0f * Hx::DeltaTime()->GetNoScaleDeltaTime();
	//����ݒ�
	m_stickInterval = min(m_stickInterval, STICK_INTERVAL);
	//���͊Ԋu�����������Ă��邩
	bool isStickInterval = m_stickInterval >= STICK_INTERVAL;


	auto ls = Input::Analog(PAD_X_Velo2Code::Velo2_LStick);	//���X�e�B�b�N
	bool isUpLS = ls.y > 0.5f;	//�X�e�B�b�N�����
	bool isDownLS = ls.y < -0.5f;//�X�e�B�b�N������
	bool isStartKey = Input::Trigger(PAD_X_KeyCode::Button_B);//B�{�^��

	if (!m_isItimie) {
		//lerp�̌v�Z
		m_lerpTimers[1] += m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
		m_lerpTimers[2] -= m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
		
		
		if (isUpLS && isStickInterval) { 
			m_num--;
			m_stickInterval = 0.0f;
			SE(SoundManager::SoundSE_ID::Enum::Cursour);
		}
		if (isDownLS && isStickInterval) { 
			m_num++; 
			m_stickInterval = 0.0f;
			SE(SoundManager::SoundSE_ID::Enum::Cursour);
		}

		if (Input::Trigger(KeyCode::Key_UP)) {
			m_num--;
			m_stickInterval = 0.0f;
			SE(SoundManager::SoundSE_ID::Enum::Cursour);
		}
		if (Input::Trigger(KeyCode::Key_DOWN)) {
			m_num++;
			m_stickInterval = 0.0f;
			SE(SoundManager::SoundSE_ID::Enum::Cursour);
		}
		//0�`3�̊ԂɃN�����v
		m_num = min(max(0, m_num), 3);

		
		//���ꂼ��̔������N����
		if (Input::Trigger(KeyCode::Key_SPACE) || isStartKey) MenuReAction(m_num);

		//�͂��L��,���W�ړ�
		m_kakoi->Enable();
		XMVECTOR pos = m_objMap[1][m_num]->m_texObj->mTransform->Position();
		pos.z += 1.0f;
		m_kakoi->mTransform->Position(pos);
	}
	else {
		//lerp�̌v�Z
		m_lerpTimers[1] -= m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
		m_lerpTimers[2] += m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();

		//�߂ɂ�[�����
		if (Input::Trigger(KeyCode::Key_SPACE) || isStartKey) MenuReAction(4); 

		//�͂�����
		m_kakoi->Disable();
	}

	//����̃|�[�Y�e�N�X�`��
	m_lerpTimers[0] += m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();

	//�N�����v����
	m_lerpTimers[0] = min(max(0.0f, m_lerpTimers[0]), 1.0f);
	m_lerpTimers[1] = min(max(0.0f, m_lerpTimers[1]), 1.0f);
	m_lerpTimers[2] = min(max(0.0f, m_lerpTimers[2]), 1.0f);


	/*���W�ړ�*/
	LerpFunc(m_objMap[1][3], 1.0f);
	for (int i = 0; i < m_objMap.size(); i++)
		//����̃e�N�X�`���̂ݏ����Œ�
		for (int j = 0; j < m_objMap[i].size(); j++)
			LerpFunc(m_objMap[i][j], m_lerpTimers[i]);
	//�E���̃e�N�X�`��
	LerpFunc(m_objMap[0][0], m_lerpTimers[0]);


	//�t�F�[�h�C��
	auto material = m_texBlack->GetComponent<MaterialComponent>();
	XMFLOAT4 color = material->GetMaterialPtr(0)->GetAlbedo();
	color.w += m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
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
			tempPos.x -= 100.0f * Hx::DeltaTime()->GetNoScaleDeltaTime();
			//x���W��0�������Ȃ��悤�ɏ���
			if (m_objMap[i][j]->m_texObj->mTransform->Position().x < 0.0f) {
				tempPos = m_objMap[i][j]->m_texObj->mTransform->Position();
				tempPos.x = 0.0f;
			}
			m_objMap[i][j]->m_texObj->mTransform->Position(tempPos);
			//�}�e���A���̃J���[�@�A���t�@�[�l���Z
			auto mat = m_objMap[i][j]->m_texObj->GetComponent<MaterialComponent>();
			XMFLOAT4 tempColor = mat->GetMaterialPtr(0)->GetAlbedo();
			tempColor.w -= m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
			if (tempColor.w <= 0.0f) tempColor.w = 0.0f;
			mat->SetAlbedoColor(tempColor);
		}
	}

	//�t�F�[�h�A�E�g
	auto material = m_texBlack->GetComponent<MaterialComponent>();
	XMFLOAT4 color = material->GetMaterialPtr(0)->GetAlbedo();
	color.w -= m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
	if (color.w <= 0.0f) color.w = 0.0f;
	material->SetAlbedoColor(color);

}

//�|�[�Y���j���[���J�����ۂ̏���
void PauseMenuManager::OpenPauseMenu(){
	if (Matinee::GlobalNowPlaying()) {
		return;
	}
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

	if (auto player = UniqueObject::GetPlayer()) {
		if (auto pc = player->GetScript<PlayerController>()) {
			pc->SetPlayerState(PlayerController::PlayerState::Movie);
		}
	}
}

//�|�[�Y���j���[�����
void PauseMenuManager::ClosePauseMenu(){
	Hx::DeltaTime()->SetTimeScale(1.0f);
	m_state = State::Close;

	if (auto player = UniqueObject::GetPlayer()) {
		if (auto pc = player->GetScript<PlayerController>()) {
			pc->SetPlayerState(PlayerController::PlayerState::Free);
		}
	}
}

//���ꂼ��̏������Ă�
void PauseMenuManager::MenuReAction(int num){
	//���艹���Đ�
	SE(SoundManager::SoundSE_ID::Enum::Decision);

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
	Hx::DeltaTime()->SetTimeScale(1.0f);
	Hx::LoadScene("Assets/Title.scene");
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

void PauseMenuManager::SE(SoundManager::SoundSE_ID::Enum key){
	XMVECTOR cameraPos = UniqueObject::GetCamera()->mTransform->Position();
	SoundManager::PlaySE(key, cameraPos);
}

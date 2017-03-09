#include "PauseMenuManager.h"
#include <h_standard.h>
#include <h_component.h>

#include "UniqueObject.h"
#include "PlayerController.h"
#include "Matinee.h"
#include "OverRayExitOption.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PauseMenuManager::Initialize(){
	m_state = State::Close;
	m_hint = Hint::one;
	m_isItimie = false;
	m_isHint = false;
	m_isHintLeftMove = false;
	m_isHintMoved = false;
	m_arrowTimer = 0.0f;
	rightArrowInitPos = rightArrow->mTransform->Position();
	leftArrowInitPos = leftArrow->mTransform->Position();
	m_num = 0;
	m_stickInterval = 0.0f;
	m_lerpTimers[0] = 0.0f;
	m_lerpTimers[1] = 0.0f;
	m_lerpTimers[2] = 0.0f;
	m_objMap[0].push_back(new Struct(m_texPause,  XMVectorSet(200,900,10001,0)));
	m_objMap[1].push_back(new Struct(m_texTips,   XMVectorSet(960,840,10001,0)));
	m_objMap[1].push_back(new Struct(m_texHowTo,  XMVectorSet(960,640,10001,0)));
	m_objMap[1].push_back(new Struct(m_texCombo,  XMVectorSet(960,440,10001,0)));
	m_objMap[1].push_back(new Struct(m_texToTitle,XMVectorSet(960,240,10001,0)));
	m_objMap[1].push_back(new Struct(m_texMigisita,XMVectorSet(1680,140,10001,0)));
	m_objMap[2].push_back(new Struct(m_texItimie, XMVectorSet(1680,540,10001,0)));
	m_texHintList.push_back(texHintOne);
	m_texHintList.push_back(texHintTwo);
	m_texHintList.push_back(texHintThree);
	m_texHintList.push_back(texHintFour);
	m_texHintList.push_back(texHintFive);

	//this->gameObject->Enable();

	for (int i = 0; i < m_objMap.size(); i++) {
		//����̃e�N�X�`���̂ݏ����Œ�
		for (int j = 0; j < m_objMap[i].size(); j++) {
			auto material = m_objMap[i][j]->m_texObj->GetComponent<MaterialComponent>();
			material->GetMaterialPtr(0)->SetAlbedo(XMFLOAT4(1, 1, 1, 0));
		}
	}
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PauseMenuManager::Start(){
	if (m_overray_exit_option) {
		if (auto src = m_overray_exit_option->GetScript<OverRayExitOption>()) {
			src->InitCall();
			src->SetFuncType(1);
		}
		m_overray_exit_option->Disable();
	}
}

//���t���[���Ă΂�܂�
void PauseMenuManager::Update() {
	if (m_overray_exit_option->IsEnabled())return;
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
	bool isRightLS = ls.x > 0.5f;//�X�e�B�b�N�E����
	bool isLeftLS = ls.x < -0.5f;//�X�e�B�b�N������
	bool isUpKey = Input::Trigger(KeyCode::Key_UP) || Input::Trigger(PAD_X_KeyCode::Button_UP);		//��L�[
	bool isDownKey = Input::Trigger(KeyCode::Key_DOWN) || Input::Trigger(PAD_X_KeyCode::Button_DOWN);//���L�[
	bool isRightKey = Input::Trigger(KeyCode::Key_RIGHT) || Input::Trigger(PAD_X_KeyCode::Button_RIGHT);//�E�L�[
	bool isLeftKey = Input::Trigger(KeyCode::Key_LEFT) || Input::Trigger(PAD_X_KeyCode::Button_LEFT);//���L�[
	bool isStartKey = Input::Trigger(PAD_X_KeyCode::Button_B);//B�{�^��
	bool isPad_A_Key = Input::Trigger(PAD_X_KeyCode::Button_A);//A�{�^��

	if (m_isHint) {
		m_lerpTimers[1] -= m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
		m_kakoi->Disable();
		for (auto i : m_texHintList) {
			i->Enable();
		}
		if (m_isHintMoved) {
			rightArrow->Enable();
			leftArrow->Enable();
		}
		else {
			rightArrow->Disable();
			leftArrow->Disable();
		}
		m_arrowTimer += 1.0f * Hx::DeltaTime()->GetNoScaleDeltaTime();
		float rightTemp = rightArrowInitPos.x + std::sin(m_arrowTimer) * 100.0f;
		float leftTemp = leftArrowInitPos.x + std::sin(-m_arrowTimer) * 100.0f;
		rightArrow->mTransform->Position(XMVectorSet(rightTemp, rightArrowInitPos.y, rightArrowInitPos.z, 0.0f));
		leftArrow->mTransform->Position(XMVectorSet(leftTemp, leftArrowInitPos.y, leftArrowInitPos.z, 0.0f));


		//�y�[�W�߂�
		if ((isRightLS || isRightKey) && m_isHintMoved) {
			m_isHintLeftMove = false;
			m_isHintMoved = false;
			m_preHintTexObj = m_texHintList[m_hint];

			m_hint = (Hint)((int)m_hint - 1);
			if (m_hint < Hint::one) m_hint = Hint::five;
			m_curHintTexObj = m_texHintList[m_hint];

			m_curHintTexObj->mTransform->Position(XMVectorSet(-800, 540, 10001, 0));

			SE(SoundManager::SoundSE_ID::Cursour);
		}
		//�y�[�W�i��
		else if((isLeftLS || isLeftKey) && m_isHintMoved) {
			m_isHintLeftMove = true;
			m_isHintMoved = false;
			m_preHintTexObj = m_texHintList[m_hint];

			m_hint = (Hint)((int)m_hint + 1);
			if (m_hint > Hint::five) m_hint = Hint::one;
			m_curHintTexObj = m_texHintList[m_hint];

			m_curHintTexObj->mTransform->Position(XMVectorSet(2800, 540, 10001, 0));

			SE(SoundManager::SoundSE_ID::Cursour);
		}

		if (m_isHintLeftMove) {
			XMVECTOR preObjPos = m_preHintTexObj->mTransform->Position();
			preObjPos.x -= 4000.0f * Hx::DeltaTime()->GetNoScaleDeltaTime();
			if (preObjPos.x <= -800.0f) {
				preObjPos.x = -800.0f;
			}
			m_preHintTexObj->mTransform->Position(preObjPos);

			XMVECTOR curObjPos = m_curHintTexObj->mTransform->Position();
			curObjPos.x -= 4000.0f * Hx::DeltaTime()->GetNoScaleDeltaTime();
			if (curObjPos.x <= 960.0f) {
				curObjPos.x = 960.0f;
				m_isHintMoved = true;
			}
			m_curHintTexObj->mTransform->Position(curObjPos);
		}
		else {
			XMVECTOR preObjPos = m_preHintTexObj->mTransform->Position();
			preObjPos.x += 4000.0f * Hx::DeltaTime()->GetNoScaleDeltaTime();
			if (preObjPos.x >= 2800.0f) {
				preObjPos.x = 2800.0f;
			}
			m_preHintTexObj->mTransform->Position(preObjPos);

			XMVECTOR curObjPos = m_curHintTexObj->mTransform->Position();
			curObjPos.x += 4000.0f * Hx::DeltaTime()->GetNoScaleDeltaTime();
			if (curObjPos.x >= 960.0f) {
				curObjPos.x = 960.0f;
				m_isHintMoved = true;
			}
			m_curHintTexObj->mTransform->Position(curObjPos);
		}



		//�߂ɂ�[�����
		if (Input::Trigger(KeyCode::Key_SPACE) || isStartKey || isPad_A_Key) MenuReAction(5);
	}
	else if (!m_isItimie) {
		//lerp�̌v�Z
		m_lerpTimers[1] += m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
		m_lerpTimers[2] -= m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
		
		
		if (isUpLS && isStickInterval) { 
			if (m_num <= 0) return;
			m_num--;
			m_stickInterval = 0.0f;
			SE(SoundManager::SoundSE_ID::Enum::Cursour);
		}
		if (isDownLS && isStickInterval) { 
			if (m_num >= 4) return;
			m_num++; 
			m_stickInterval = 0.0f;
			SE(SoundManager::SoundSE_ID::Enum::Cursour);
		}

		if (isUpKey) {
			if (m_num <= 0) return;
			m_num--;
			m_stickInterval = 0.0f;
			SE(SoundManager::SoundSE_ID::Enum::Cursour);
		}
		if (isDownKey) {
			if (m_num >= 4) return;
			m_num++;
			m_stickInterval = 0.0f;
			SE(SoundManager::SoundSE_ID::Enum::Cursour);
		}

		if (m_lerpTimers[1] >= 1.0f) {
			if (isPad_A_Key) ClosePauseMenu();
		}

		//0�`3�̊ԂɃN�����v
		m_num = min(max(0, m_num), 4);
		
		//���ꂼ��̔������N����
		if (Input::Trigger(KeyCode::Key_SPACE) || isStartKey) MenuReAction(m_num);

		//�͂��L��,���W�ړ�
		m_kakoi->Enable();
		XMVECTOR pos = m_objMap[1][m_num]->m_texObj->mTransform->Position();
		pos.z += 1.0f;
		m_kakoi->mTransform->Position(pos);

		for (auto i : m_texHintList) {
			i->Disable();
		}
	}
	else {
		//lerp�̌v�Z
		m_lerpTimers[1] -= m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
		m_lerpTimers[2] += m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();

		//�߂ɂ�[�����
		if (Input::Trigger(KeyCode::Key_SPACE) || isStartKey || isPad_A_Key) MenuReAction(5); 

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
	m_Camvas->Enable();
	m_state = State::Open;
	m_isItimie = false;
	m_isHint = false;
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
			pc->SetPlayerLock(true);
		}
	}

	rightArrow->Disable();
	leftArrow->Disable();
}

//�|�[�Y���j���[�����
void PauseMenuManager::ClosePauseMenu(){
	Hx::DeltaTime()->SetTimeScale(1.0f);
	m_state = State::Close;
	m_isHint = false;
	rightArrow->Disable();
	leftArrow->Disable();

	if (auto player = UniqueObject::GetPlayer()) {
		if (auto pc = player->GetScript<PlayerController>()) {
			pc->SetPlayerLock(false);
		}
	}

	for (auto i : m_texHintList) {
		i->Disable();
	}
}

//���ꂼ��̏������Ă�
void PauseMenuManager::MenuReAction(int num){
	//���艹���Đ�
	SE(SoundManager::SoundSE_ID::Enum::Decision);

	switch (num)
	{
	case 0:	OnHint();
		break;
	case 1:	ChangeItimaie(Itimie::HowTo);	//������@�ɐ؂�ւ���
		break;
	case 2:	ChangeItimaie(Itimie::Combo);	//�R���{�\�ɐ؂�ւ���
		break;
	case 3: BackToTitle();		//�^�C�g���ɔ��
		break;
	case 4: ClosePauseMenu();	//���j���[�����
		break;
	case 5: BackMenu();			//1���G���烁�j���[�ɖ߂�
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
	switch (type){
	case PauseMenuManager::HowTo: texItimaie = texHowTo;
		break;
	case PauseMenuManager::Combo: texItimaie = texCombo;
		break;
	}

	auto material1 = m_objMap[2][0]->m_texObj->GetComponent<MaterialComponent>();
	material1->GetMaterialPtr(0)->SetTexture(texItimaie);
	auto material2 = m_objMap[1][4]->m_texObj->GetComponent<MaterialComponent>();
	material2->GetMaterialPtr(0)->SetTexture(texBack);

}

//1���G���烁�j���[���X�g�ɖ߂�
void PauseMenuManager::BackMenu(){
	if (m_objMap[1][3] == nullptr) return;
	m_isItimie = false;
	m_isHint = false;
	rightArrow->Disable();
	leftArrow->Disable();
	auto material = m_objMap[1][4]->m_texObj->GetComponent<MaterialComponent>();
	material->GetMaterialPtr(0)->SetTexture(texClose);
}

//�^�C�g���ɔ��
void PauseMenuManager::BackToTitle(){
	if (!m_overray_exit_option->IsEnabled()) {
		m_overray_exit_option->Enable();
		m_overray_exit_option->GetScript<OverRayExitOption>()->SetOverrayFlag(true);

	}
	//Hx::DeltaTime()->SetTimeScale(1.0f);
	//Hx::LoadScene("Assets/Title.scene");
}

void PauseMenuManager::OnHint(){
	m_hint = Hint::one;
	m_isHint = true;
	m_isHintLeftMove = true;
	m_preHintTexObj = m_texHintList[Hint::five];
	m_curHintTexObj = m_texHintList[Hint::one];

	for (auto i : m_texHintList) {
		i->mTransform->Position(XMVectorSet(2800,540,10001,0));
	}

	m_preHintTexObj->mTransform->Position(XMVectorSet(-600, 540,10001,0));
	m_curHintTexObj->mTransform->Position(XMVectorSet(2800, 540,10001,0));
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
	SoundManager::PlaySE_2D(key);
}

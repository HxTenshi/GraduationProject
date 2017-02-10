#include "GameClearScene.h"


/****************************************************

	GameClaerTexObj

*****************************************************/
namespace Mossan{

	GameClearTexObj::GameClearTexObj(GameObject gameObject) {
		m_texObj = gameObject;
		m_initScale = m_texObj->mTransform->Scale();
		m_isEnd = false;
	}

	GameClearTexObj::GameClearTexObj() {
	}

	void GameClearTexObj::WaitSmall(){
		Disable();
		XMVECTOR scale = m_initScale;
		scale.x = 0.0f;
		m_texObj->mTransform->Scale(scale);
	}

	void GameClearTexObj::WaitBig(){
		const float scaleRate = 2.0f;

		Disable();
		XMVECTOR scale = m_initScale;
		scale *= scaleRate;
		m_texObj->mTransform->Scale(scale);
	}

	void GameClearTexObj::Wipe(){
		//�g�傷��l
		const float addScaleRate = 2.0f;
		//�L����
		Enable();
		//���݂̃X�P�[���l
		XMVECTOR scale = m_texObj->mTransform->Scale();
		//x���̃X�P�[���l�ɉ��Z
		scale.x += m_initScale.x * addScaleRate * Hx::DeltaTime()->GetDeltaTime();
		//�����X�P�[�����傫���Ȃ����珉���X�P�[���ŌŒ�
		if (scale.x > m_initScale.x) { 
			scale = m_initScale; 
			m_isEnd = true;
		}
		//�X�P�[���l������
		m_texObj->mTransform->Scale(scale);
	}

	void GameClearTexObj::Small(){
		//�k�����銄��
		const float subScaleRate = 4.0f;
		//�L����
		Enable();
		//���݂̃X�P�[���l
		XMVECTOR scale = m_texObj->mTransform->Scale();
		//x���̃X�P�[���l�ɉ��Z
		scale -= m_initScale * subScaleRate * Hx::DeltaTime()->GetDeltaTime();
		//�����X�P�[�����傫���Ȃ����珉���X�P�[���ŌŒ�
		if (scale.x < m_initScale.x) { 
			scale = m_initScale; 
			m_isEnd = true;
		}
		//�X�P�[���l������
		m_texObj->mTransform->Scale(scale);
	}

	void GameClearTexObj::Enable(){
		m_texObj->Enable();
	}

	void GameClearTexObj::Disable(){
		m_texObj->Disable();
	}

	std::string GameClearTexObj::GetObjName()
	{
		return m_texObj->Name();
	}

	bool GameClearTexObj::IsEnd()
	{
		return m_isEnd;
	}

	/****************************************************

	GameClaerTextObj

	*****************************************************/

	GameClearTextObj::GameClearTextObj(GameObject gameObject){
		if (gameObject == NULL) return;
		m_textObj = gameObject;
		m_number = 0;
		m_pivotNum = 0;
		m_isEnd = false;
		ChangeText("");
	}

	GameClearTextObj::GameClearTextObj(){}

	void GameClearTextObj::UpdateScore(){
		//�X�R�A�����Z
		m_number += m_pivotNum * Hx::DeltaTime()->GetDeltaTime();
		//����𐧌�
		if (m_number > m_pivotNum) {
			m_number = m_pivotNum;
			m_isEnd = true;
		}
		//string�ϊ�
		std::string message = std::to_string(m_number);
		//�߂����[���ύX
		ChangeText(message);
	}

	void GameClearTextObj::UpdateTime() {
		//�X�R�A�����Z
		m_number += m_pivotNum * Hx::DeltaTime()->GetDeltaTime();
		//����𐧌�
		if (m_number > m_pivotNum) {
			m_number = m_pivotNum;
			m_isEnd = true;
		}
	
		int minute = m_number / 60;
		int second = m_number % 60;
		//string�ϊ�
		std::string message = std::to_string(minute) + " : " + std::to_string(second);
		//�߂����[���ύX
		ChangeText(message);
	}

	void GameClearTextObj::ChangeText(std::string message){
		auto text = m_textObj->GetComponent<TextComponent>();
		if (text == NULL) return;
		text->ChangeText(message);
	}

	void GameClearTextObj::SetScore(int num){
		m_pivotNum = num;
	}

	int GameClearTextObj::GetScore(){
		return m_pivotNum;
	}

	bool GameClearTextObj::IsEnd(){
		return m_isEnd;
	}


};



/****************************************************

	GameClaerScene

*****************************************************/

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GameClearScene::Initialize(){
	m_texClearObj = Mossan::GameClearTexObj(m_texClear);
	m_texKillObj = Mossan::GameClearTexObj(m_texKill);
	m_texTimeObj = Mossan::GameClearTexObj(m_texTime);
	m_textKillObj = Mossan::GameClearTextObj(m_textKill);
	m_textTimeObj = Mossan::GameClearTextObj(m_textTime);
	//�ҋ@��Ԃ��w��
	m_texClearObj.WaitSmall();
	m_texKillObj.WaitBig();
	m_texTimeObj.WaitBig();
	m_textKillObj.SetScore(200);
	m_textTimeObj.SetScore(150);
	m_phase = Phase::phase1;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GameClearScene::Start(){
}

//���t���[���Ă΂�܂�
void GameClearScene::Update(){
	if (m_texClear == NULL) return;
	if (m_texKill == NULL) return;
	if (m_texTime == NULL) return;
	if (m_textKill == NULL) return;
	if (m_textTime == NULL) return;



	if (m_phase == phase1) DoPhase1();
	else if (m_phase == phase2) DoPhase2();
	else if (m_phase == phase3) DoPhase3();
	else if (m_phase == phase4) DoPhase4();
	
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void GameClearScene::Finish(){
	
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GameClearScene::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GameClearScene::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void GameClearScene::OnCollideExit(GameObject target){
	(void)target;
}

void GameClearScene::DoPhase1(){
	//clear���S�����Ɋg��
	m_texClearObj.Wipe();
	//�I������Ȃ�phase2��
	if (m_texClearObj.IsEnd()) {
		m_phase = phase2;
	}
}

void GameClearScene::DoPhase2(){
	//�e�N�X�`�����k��
	m_texKillObj.Small();
	//�k���I������Ȃ�
	if (m_texKillObj.IsEnd()) {
		//�كJ�E���g�i�߂�
		m_textKillObj.UpdateScore();
		//�I������Ȃ�phase3��
		if (m_textKillObj.IsEnd()) {
			m_phase = phase3;
		}
	}
}

void GameClearScene::DoPhase3(){
	//�e�N�X�`�����k��
	m_texTimeObj.Small();
	//�k���I������Ȃ�
	if (m_texTimeObj.IsEnd()) {
		m_textTimeObj.UpdateTime();

		if (m_textTimeObj.IsEnd()) {
			m_phase = phase4;
		}
	}
}

void GameClearScene::DoPhase4(){
	if (Input::Trigger(KeyCode::Key_SPACE)) {
		Hx::LoadScene("TitleScene.scene");
	}
}

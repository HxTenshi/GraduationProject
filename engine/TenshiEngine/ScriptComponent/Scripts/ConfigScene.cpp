#include "ConfigScene.h"
#include "h_standard.h"
#include "h_component.h"

ConfigScene::ConfigScene() {
	
}

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ConfigScene::Initialize(){
	m_texObjs.push_back(m_volumeTexObj);
	m_texObjs.push_back(m_howToTexObj);
	m_texObjs.push_back(m_comboTexObj);
	m_texObjs.push_back(m_toTitleTexObj);
	num = 0;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ConfigScene::Start(){
}

//���t���[���Ă΂�܂�
void ConfigScene::Update(){
	//�J�[�\���̈ړ�
	CursourMove();
	//�V�[���̌���
	EnterScene(num);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ConfigScene::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ConfigScene::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ConfigScene::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void ConfigScene::OnCollideExit(GameObject target){
	(void)target;
}

//�J�[�\���̈ړ�
void ConfigScene::CursourMove(){
	//null�`�F�b�N
	if (m_texObjs[0] == NULL)return;
	if (m_texObjs[1] == NULL)return;
	if (m_texObjs[2] == NULL)return;
	if (m_texObjs[3] == NULL)return;


	if (Input::Trigger(KeyCode::Key_UP)) num--;
	if (Input::Trigger(KeyCode::Key_DOWN)) num++;

	//�N�����v����
	int minNum = 0;
	int maxNum = m_texObjs.size() - 1;
	num = (int)min(max(minNum, num), maxNum);

	//�͂��̍��W�ړ�
	XMVECTOR kakoiPos = m_texObjs[num]->mTransform->Position();
	m_kakoiTexObj->mTransform->Position(kakoiPos);
}

//�V�[���̌���
void ConfigScene::EnterScene(int num){
	if (Input::Trigger(KeyCode::Key_SPACE)) {
		std::string nextScenePass = GetScenePass(num);
		Hx::Debug()->Log(nextScenePass + "�ɑJ�ڂ��܂���");
		Hx::LoadScene(nextScenePass);
	}
}

//�V�[���̃p�X���擾
std::string ConfigScene::GetScenePass(int num){
	std::string sceneNamesPass[] = {
		m_volumeScenePass,
		m_howtoScenePass,
		m_comboScenePass,
		m_titleScenePass
	};
	return sceneNamesPass[num];
}

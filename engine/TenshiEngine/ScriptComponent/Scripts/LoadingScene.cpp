#include "LoadingScene.h"
#include "FuckTenshiEngine.h"
#include "h_standard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void LoadingScene::Initialize(){
	isload = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void LoadingScene::Start(){
	
}

//���t���[���Ă΂�܂�
void LoadingScene::Update(){
	if (isload) return;
	isload = true;
	std::vector<std::vector<std::string>> arrays = Mossan::CSVScript::readCSV("Assets/data/NextScenePass.csv");
	Hx::Debug()->Log(arrays[0][0] + "�ɑJ�ڂ��܂�");
	Hx::LoadScene(arrays[0][0]);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void LoadingScene::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void LoadingScene::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void LoadingScene::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void LoadingScene::OnCollideExit(GameObject target){
	(void)target;
}
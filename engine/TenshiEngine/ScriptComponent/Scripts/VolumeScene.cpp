#include "h_standard.h""
#include "VolumeScene.h"
#include "SoundManager.h"
#include "FuckTenshiEngine.h"
#include <istream>
#include <fstream>
#include <sstream>

enum Enum {
	BGM = 0,
	SE,
	BackConfig
};

using namespace std;
using namespace Mossan;

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void VolumeScene::Initialize(){
	if (!m_bgmBarObj) return;
	if (!m_seBarObj) return;
	if (!m_backConfigObj) return;
	////���ʂ�ۑ����Ă���CSV��ǂݍ���
	vector<vector<string>> arrays = CSVScript::readCSV("Assets/data/Volume.csv");
	//BGM�̏����擾
	bgmCtrl = m_bgmBarObj->GetScript<VolumeBarCtrl>();
	bgmCtrl->SetVolume(atoi(arrays[0][0].c_str()));
	//SE�̉��ʂ��擾
	seCtrl = m_seBarObj->GetScript<VolumeBarCtrl>();
	seCtrl->SetVolume(atoi(arrays[0][1].c_str()));

	m_objList.push_back(m_bgmBarObj);
	m_objList.push_back(m_seBarObj);
	m_objList.push_back(m_backConfigObj);
	num = 0;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void VolumeScene::Start(){

}

//���t���[���Ă΂�܂�
void VolumeScene::Update(){
	//Null�`�F�b�N
	if (m_objList[Enum::BGM] == NULL) return;
	if (m_objList[Enum::SE] == NULL) return;
	if (m_objList[Enum::BackConfig] == NULL) return;

	if (Input::Trigger(KeyCode::Key_UP)) num--;
	if (Input::Trigger(KeyCode::Key_DOWN)) num++;

	//�{�����[���̒���
	if (Input::Trigger(KeyCode::Key_LEFT)) {
		if (num >= Enum::BackConfig) return;
		auto barCtrl = m_objList[num]->GetScript<VolumeBarCtrl>();
		//Null�`�F�b�N
		if (!barCtrl) return;
		//���ʉ�����
		barCtrl->VolumeDown();
	}else if (Input::Trigger(KeyCode::Key_RIGHT)) {
		if (num >= Enum::BackConfig) return;
		auto barCtrl = m_objList[num]->GetScript<VolumeBarCtrl>();
		//Null�`�F�b�N
		if (!barCtrl) return;
		//���ʂ��グ��
		barCtrl->VolumeUp();
	}

	//Config�ɖ߂�
	else if (Input::Trigger(KeyCode::Key_SPACE)) {
		if (num != Enum::BackConfig) return;
		//����
		Decision();
	}



	//�N�����v����
	int minNum = 0;
	int maxNum = m_objList.size() - 1;
	num = (int)min(max(minNum, num), maxNum);

	//�͂��̍��W�ړ�
	XMVECTOR kakoiPos = m_objList[num]->mTransform->Position();
	m_kakoi->mTransform->Position(kakoiPos);
}


//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void VolumeScene::Finish(){
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void VolumeScene::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void VolumeScene::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void VolumeScene::OnCollideExit(GameObject target){
	(void)target;
}


//�ݒ�̌��莞�̏���
void VolumeScene::Decision(){
	int bgm = bgmCtrl->GetVolume();
	int se = seCtrl->GetVolume();

	writeCSV("Assets/data/Volume.csv", bgm, se);
	Hx::LoadScene("Assets/Mossan/Config.scene");
}


void VolumeScene::writeCSV(std::string fileName, int bgmVolume, int seVolume){
	//�t�@�C�����J��
	ofstream ofs(fileName);
	//BGM,SE�̉��ʂ�string�^�ɕϊ�
	std::string bgm = std::to_string(bgmVolume);
	std::string se = std::to_string(seVolume);
	//��������
	ofs << bgm << "," << se << endl;
}

std::vector<std::vector<std::string>> VolumeScene::readCSV(std::string fileName){
	std::vector<std::vector<std::string>> arrays;
	//�t�@�C�����J��
	std::ifstream ifs(fileName);
	if (!ifs) {
		Hx::Debug()->Log("CSV�̖��O���Ⴄ");
	}

	//CSV�t�@�C����1�s���ǂݍ���
	std::string str;
	std::vector<std::string> p;
	//1��ǂݍ���
	while (std::getline(ifs, str)) {
		std::string token;
		std::istringstream stream(str);
		//�s�ɕ�����
		while (std::getline(stream, token, ',')) {
			//vector�ɓ����
			p.push_back(token);
		}
		arrays.push_back(p);
	}
	return arrays;
}

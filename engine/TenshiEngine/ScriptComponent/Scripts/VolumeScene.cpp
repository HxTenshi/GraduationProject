#include "h_standard.h""
#include "VolumeScene.h"
#include "Fader.h"
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

	m_stickIntervalTime = 0.0f;
	m_stickTilTimer = 0.0f;
	m_continuTimer = 0.0f;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void VolumeScene::Start(){
	SoundManager::PlayBGM(SoundManager::SoundBGM_ID::Credit);
}

//���t���[���Ă΂�܂�
void VolumeScene::Update(){
	//Null�`�F�b�N
	if (m_objList[Enum::BGM] == NULL) return;
	if (m_objList[Enum::SE] == NULL) return;
	if (m_objList[Enum::BackConfig] == NULL) return;

	m_stickIntervalTime += 1.0f * Hx::DeltaTime()->GetDeltaTime();
	m_stickIntervalTime = min(m_stickIntervalTime, 1.0f);
	bool interval = m_stickIntervalTime > STICK_INTERVAL;

	auto ls = Input::Analog(PAD_X_Velo2Code::Velo2_LStick);
	bool isUp = ls.y > 0.5f;
	bool isDown = ls.y < -0.5f;
	bool isRight = ls.x > 0.5f;
	bool isLeft = ls.x < -0.5f;
	bool isEnter = Input::Trigger(PAD_X_KeyCode::Button_B);

	bool isFirstTilt = m_stickTilTimer == 0.0f;
	bool isContinueTilt = m_stickTilTimer > 0.5f && m_continuTimer > 0.05f;
	if (isFirstTilt || isContinueTilt) {
		m_continuTimer = 0.0f;

		//�{�����[���̒���
		if ((Input::Down(KeyCode::Key_LEFT) || isLeft)) {
			if (num >= Enum::BackConfig) return;
			if (m_objList[num]->GetScript<VolumeBarCtrl>()->IsVolumeMin()) return;
			auto barCtrl = m_objList[num]->GetScript<VolumeBarCtrl>();
			//Null�`�F�b�N
			if (!barCtrl) return;
			//���ʉ�����
			barCtrl->VolumeDown();

			OnSE(SoundManager::SoundSE_ID::Enum::VolumeDown);
		}
		else if ((Input::Down(KeyCode::Key_RIGHT) || isRight)) {
			if (num >= Enum::BackConfig) return;
			if (m_objList[num]->GetScript<VolumeBarCtrl>()->IsVolumeMax()) return;
			auto barCtrl = m_objList[num]->GetScript<VolumeBarCtrl>();
			//Null�`�F�b�N
			if (!barCtrl) return;
			//���ʂ��グ��
			barCtrl->VolumeUp();

			OnSE(SoundManager::SoundSE_ID::Enum::VolumeUp);
		}
	}

	if (isRight || isLeft || Input::Down(KeyCode::Key_RIGHT) || Input::Down(KeyCode::Key_LEFT)) {
		m_stickTilTimer += 1.0f * Hx::DeltaTime()->GetDeltaTime();
		m_continuTimer += 1.0f * Hx::DeltaTime()->GetDeltaTime();
	}
	else {
		m_stickTilTimer = 0.0f;
		m_continuTimer = 0.0f;
	}

	

	if ((Input::Trigger(KeyCode::Key_UP) || isUp) && interval) {
		//if (num <= 0) return;
		num--;
		m_stickIntervalTime = 0.0f;
		OnSE(SoundManager::SoundSE_ID::Enum::Cursour);
	}
	if ((Input::Trigger(KeyCode::Key_DOWN) || isDown) && interval) {
		//if (num <= m_objList.size() -1) return;
		num++;
		m_stickIntervalTime = 0.0f;
		OnSE(SoundManager::SoundSE_ID::Enum::Cursour);
	}

	//Config�ɖ߂�
	else if (Input::Trigger(KeyCode::Key_SPACE) || isEnter) {
		if (num != Enum::BackConfig) return;
		OnSE(SoundManager::SoundSE_ID::Enum::Decision);
		//����
		Decision();
	}

	float bgmVolume = m_objList[Enum::BGM]->GetScript<VolumeBarCtrl>()->GetVolume();
	SoundManager::SetBGMVolume(bgmVolume);

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

	if (!m_fader) return;
	auto fader = m_fader->GetScript<Fader>();
	fader->OnSceneChnage("Assets/Mossan/Config.scene");
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

void VolumeScene::OnSE(SoundManager::SoundSE_ID::Enum key){
	float seVolume = m_objList[Enum::SE]->GetScript<VolumeBarCtrl>()->GetVolume();
	SoundManager::PlaySE(key, XMVectorZero(), seVolume);
}

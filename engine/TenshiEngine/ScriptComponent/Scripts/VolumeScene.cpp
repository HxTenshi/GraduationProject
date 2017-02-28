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

//生成時に呼ばれます（エディター中も呼ばれます）
void VolumeScene::Initialize(){
	if (!m_bgmBarObj) return;
	if (!m_seBarObj) return;
	if (!m_backConfigObj) return;
	////音量を保存しているCSVを読み込む
	vector<vector<string>> arrays = CSVScript::readCSV("Assets/data/Volume.csv");
	//BGMの情報を取得
	bgmCtrl = m_bgmBarObj->GetScript<VolumeBarCtrl>();
	bgmCtrl->SetVolume(atoi(arrays[0][0].c_str()));
	//SEの音量を取得
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

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void VolumeScene::Start(){
	SoundManager::PlayBGM(SoundManager::SoundBGM_ID::Credit);
}

//毎フレーム呼ばれます
void VolumeScene::Update(){
	//Nullチェック
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

		//ボリュームの調整
		if ((Input::Down(KeyCode::Key_LEFT) || isLeft)) {
			if (num >= Enum::BackConfig) return;
			if (m_objList[num]->GetScript<VolumeBarCtrl>()->IsVolumeMin()) return;
			auto barCtrl = m_objList[num]->GetScript<VolumeBarCtrl>();
			//Nullチェック
			if (!barCtrl) return;
			//音量下げる
			barCtrl->VolumeDown();

			OnSE(SoundManager::SoundSE_ID::Enum::VolumeDown);
		}
		else if ((Input::Down(KeyCode::Key_RIGHT) || isRight)) {
			if (num >= Enum::BackConfig) return;
			if (m_objList[num]->GetScript<VolumeBarCtrl>()->IsVolumeMax()) return;
			auto barCtrl = m_objList[num]->GetScript<VolumeBarCtrl>();
			//Nullチェック
			if (!barCtrl) return;
			//音量を上げる
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

	//Configに戻る
	else if (Input::Trigger(KeyCode::Key_SPACE) || isEnter) {
		if (num != Enum::BackConfig) return;
		OnSE(SoundManager::SoundSE_ID::Enum::Decision);
		//決定
		Decision();
	}

	float bgmVolume = m_objList[Enum::BGM]->GetScript<VolumeBarCtrl>()->GetVolume();
	SoundManager::SetBGMVolume(bgmVolume);

	//クランプ処理
	int minNum = 0;
	int maxNum = m_objList.size() - 1;
	num = (int)min(max(minNum, num), maxNum);

	//囲いの座標移動
	XMVECTOR kakoiPos = m_objList[num]->mTransform->Position();
	m_kakoi->mTransform->Position(kakoiPos);
}


//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void VolumeScene::Finish(){
}

//コライダーとのヒット時に呼ばれます
void VolumeScene::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void VolumeScene::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void VolumeScene::OnCollideExit(GameObject target){
	(void)target;
}


//設定の決定時の処理
void VolumeScene::Decision(){
	int bgm = bgmCtrl->GetVolume();
	int se = seCtrl->GetVolume();

	writeCSV("Assets/data/Volume.csv", bgm, se);

	if (!m_fader) return;
	auto fader = m_fader->GetScript<Fader>();
	fader->OnSceneChnage("Assets/Mossan/Config.scene");
}


void VolumeScene::writeCSV(std::string fileName, int bgmVolume, int seVolume){
	//ファイルを開く
	ofstream ofs(fileName);
	//BGM,SEの音量をstring型に変換
	std::string bgm = std::to_string(bgmVolume);
	std::string se = std::to_string(seVolume);
	//書き込み
	ofs << bgm << "," << se << endl;
} 

std::vector<std::vector<std::string>> VolumeScene::readCSV(std::string fileName){
	std::vector<std::vector<std::string>> arrays;
	//ファイルを開く
	std::ifstream ifs(fileName);
	if (!ifs) {
		Hx::Debug()->Log("CSVの名前が違う");
	}

	//CSVファイルを1行ずつ読み込む
	std::string str;
	std::vector<std::string> p;
	//1列読み込む
	while (std::getline(ifs, str)) {
		std::string token;
		std::istringstream stream(str);
		//行に分ける
		while (std::getline(stream, token, ',')) {
			//vectorに入れる
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

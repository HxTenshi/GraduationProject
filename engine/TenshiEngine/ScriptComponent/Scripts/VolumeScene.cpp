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
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void VolumeScene::Start(){

}

//毎フレーム呼ばれます
void VolumeScene::Update(){
	//Nullチェック
	if (m_objList[Enum::BGM] == NULL) return;
	if (m_objList[Enum::SE] == NULL) return;
	if (m_objList[Enum::BackConfig] == NULL) return;

	if (Input::Trigger(KeyCode::Key_UP)) num--;
	if (Input::Trigger(KeyCode::Key_DOWN)) num++;

	//ボリュームの調整
	if (Input::Trigger(KeyCode::Key_LEFT)) {
		if (num >= Enum::BackConfig) return;
		auto barCtrl = m_objList[num]->GetScript<VolumeBarCtrl>();
		//Nullチェック
		if (!barCtrl) return;
		//音量下げる
		barCtrl->VolumeDown();
	}else if (Input::Trigger(KeyCode::Key_RIGHT)) {
		if (num >= Enum::BackConfig) return;
		auto barCtrl = m_objList[num]->GetScript<VolumeBarCtrl>();
		//Nullチェック
		if (!barCtrl) return;
		//音量を上げる
		barCtrl->VolumeUp();
	}

	//Configに戻る
	else if (Input::Trigger(KeyCode::Key_SPACE)) {
		if (num != Enum::BackConfig) return;
		//決定
		Decision();
	}



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
	Hx::LoadScene("Assets/Mossan/Config.scene");
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

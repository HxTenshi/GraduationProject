#include "GameClearScene.h"
#include "Fader.h"
#include "FuckTenshiEngine.h"
#include "SoundManager.h"

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
		//拡大する値
		const float addScaleRate = 2.0f;
		//有効化
		Enable();
		//現在のスケール値
		XMVECTOR scale = m_texObj->mTransform->Scale();
		//x軸のスケール値に加算
		scale.x += m_initScale.x * addScaleRate * Hx::DeltaTime()->GetDeltaTime();
		//初期スケールより大きくなったら初期スケールで固定
		if (scale.x > m_initScale.x) { 
			scale = m_initScale; 
			m_isEnd = true;
		}
		//スケール値を決定
		m_texObj->mTransform->Scale(scale);
	}

	void GameClearTexObj::Small(){
		//縮小する割合
		const float subScaleRate = 2.0f;
		//有効化
		Enable();
		//現在のスケール値
		XMVECTOR scale = m_texObj->mTransform->Scale();
		//x軸のスケール値に加算
		scale -= m_initScale * subScaleRate * Hx::DeltaTime()->GetDeltaTime();
		//初期スケールより大きくなったら初期スケールで固定
		if (scale.x < m_initScale.x) { 
			scale = m_initScale; 
			m_isEnd = true;
		}
		//スケール値を決定
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
		m_numberf = 0.0f;
		m_number = 0;
		m_pivotNum = 0;
		m_isEnd = false;
		m_isSE = false;
		ChangeText("");
	}

	GameClearTextObj::GameClearTextObj(){}

	void GameClearTextObj::UpdateScore(){
		if (!m_isSE) {
			m_isSE = true;
			SoundManager::PlaySE_2D(SoundManager::SoundSE_ID::Enum::DrumRoll);
		}

		//スコアを加算
		m_numberf += m_pivotNum * Hx::DeltaTime()->GetDeltaTime();
		m_number = (int)m_numberf;
		//上限を制限
		if (m_number > m_pivotNum) {
			m_number = m_pivotNum;
			m_isEnd = true;
		}
		//string変換
		std::string message = std::to_string(m_number);
		//めっさーじ変更
	std::vector<std::vector<std::string>> scoreVec = Mossan::CSVScript::readCSV("Assets/data/Score.csv");
		ChangeText(message + "/" + scoreVec[0][1]);
	}

	void GameClearTextObj::UpdateTime() {
		if (!m_isSE) {
			m_isSE = true;
			SoundManager::PlaySE_2D(SoundManager::SoundSE_ID::Enum::DrumRoll);
		}

		//スコアを加算
		m_numberf += m_pivotNum * Hx::DeltaTime()->GetDeltaTime();
		m_number = (int)m_numberf;
		//上限を制限
		if (m_number > m_pivotNum) {
			m_number = m_pivotNum;
			m_isEnd = true;
		}
	
		int minute = m_number / 60;
		int second = m_number % 60;
		//string変換
		std::string message = (minute < 10 ? "0" : "") + std::to_string(minute) + " : " + (second < 10 ? "0" : "") + std::to_string(second);
		//めっさーじ変更
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

//生成時に呼ばれます（エディター中も呼ばれます）
void GameClearScene::Initialize(){
	m_texClearObj = Mossan::GameClearTexObj(m_texClear);
	m_texKillObj = Mossan::GameClearTexObj(m_texKill);
	m_texTimeObj = Mossan::GameClearTexObj(m_texTime);
	m_textKillObj = Mossan::GameClearTextObj(m_textKill);
	m_textTimeObj = Mossan::GameClearTextObj(m_textTime);
	//待機状態を指定
	m_texClearObj.WaitSmall();
	m_texKillObj.WaitBig();
	m_texTimeObj.WaitBig();
	std::vector<std::vector<std::string>> scoreVec = Mossan::CSVScript::readCSV("Assets/data/Score.csv");
	m_textKillObj.SetScore(stoi(scoreVec[0][0]));
	m_textTimeObj.SetScore(stoi(scoreVec[0][2]));
	m_phase = Phase::phase1;
	m_timer = 0.0f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void GameClearScene::Start(){
	SoundManager::PlayBGM(SoundManager::SoundBGM_ID::Enum::GameClear);
}

//毎フレーム呼ばれます
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

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void GameClearScene::Finish(){
	
}

//コライダーとのヒット時に呼ばれます
void GameClearScene::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void GameClearScene::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void GameClearScene::OnCollideExit(GameObject target){
	(void)target;
}

void GameClearScene::DoPhase1(){
	//clearロゴを横に拡大
	m_texClearObj.Wipe();
	//終わったならphase2へ
	if (m_texClearObj.IsEnd()) {
		m_phase = phase2;
	}
}

void GameClearScene::DoPhase2(){
	//テクスチャを縮小
	m_texKillObj.Small();
	//縮小終わったなら
	if (m_texKillObj.IsEnd()) {
		//ｷﾙカウント進める
		m_textKillObj.UpdateScore();
		//終わったならphase3へ
		if (m_textKillObj.IsEnd()) {
			SoundManager::PlaySE_2D(SoundManager::SoundSE_ID::Enum::Simbal);
			m_phase = phase3;
		}
	}
}

void GameClearScene::DoPhase3(){
	//テクスチャを縮小
	m_texTimeObj.Small();
	//縮小終わったなら
	if (m_texTimeObj.IsEnd()) {
		m_textTimeObj.UpdateTime();

		if (m_textTimeObj.IsEnd()) {
			SoundManager::PlaySE_2D(SoundManager::SoundSE_ID::Enum::Simbal);
			m_phase = phase4;
		}
	}
}

void GameClearScene::DoPhase4(){

	bool isEnter = Input::Trigger(KeyCode::Key_SPACE) || Input::Trigger(PAD_X_KeyCode::Button_B);

	const float intervalTime = 2.1f;
	m_timer += 1.0f * Hx::DeltaTime()->GetDeltaTime();
	m_timer = min(m_timer, intervalTime);

	if (m_timer >= intervalTime || isEnter) {
		if (!m_fader) return;
		auto fader = m_fader->GetScript<Fader>();
		fader->OnSceneChnage("Assets/Mossan/Credit.scene");
	}
}
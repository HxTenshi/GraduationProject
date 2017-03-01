#include "ConfigScene.h"
#include "h_standard.h"
#include "h_component.h"
#include "Fader.h"
#include "UniqueObject.h"

ConfigScene::ConfigScene() {
	
}

//生成時に呼ばれます（エディター中も呼ばれます）
void ConfigScene::Initialize(){
	m_texObjs.push_back(m_volumeTexObj);
	m_texObjs.push_back(m_howToTexObj);
	m_texObjs.push_back(m_comboTexObj);
	m_texObjs.push_back(m_toTitleTexObj);
	num = 0;
	m_stickInterval = 0.0f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ConfigScene::Start(){
}

//毎フレーム呼ばれます
void ConfigScene::Update(){
	//カーソルの移動
	CursourMove();
	//シーンの決定
	EnterScene(num);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ConfigScene::Finish(){

}

//コライダーとのヒット時に呼ばれます
void ConfigScene::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void ConfigScene::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void ConfigScene::OnCollideExit(GameObject target){
	(void)target;
}

//カーソルの移動
void ConfigScene::CursourMove() {
	//nullチェック
	if (m_texObjs[0] == NULL)return;
	if (m_texObjs[1] == NULL)return;
	if (m_texObjs[2] == NULL)return;
	if (m_texObjs[3] == NULL)return;

	m_stickInterval += 1.0f * Hx::DeltaTime()->GetDeltaTime();
	m_stickInterval = min(m_stickInterval, 1.0f);
	bool isStickInterval = m_stickInterval > STICK_INTERVAL;

	auto ls = Input::Analog(PAD_X_Velo2Code::Velo2_LStick);
	bool isUpLS = ls.y > 0.5f;
	bool isDownLS = ls.y < -0.5f;
	if ((Input::Trigger(KeyCode::Key_UP) || isUpLS) && isStickInterval) {
		num--;
		m_stickInterval = 0.0f;
		SE(SoundManager::SoundSE_ID::Enum::Cursour);
	}
	if (Input::Trigger(KeyCode::Key_DOWN) || isDownLS && isStickInterval) {
		num++;
		m_stickInterval = 0.0f;
		SE(SoundManager::SoundSE_ID::Enum::Cursour);
	}

	//クランプ処理
	int minNum = 0;
	int maxNum = m_texObjs.size() - 1;
	num = (int)min(max(minNum, num), maxNum);

	//囲いの座標移動
	XMVECTOR kakoiPos = m_texObjs[num]->mTransform->Position();
	kakoiPos.z += 1.0f;
	m_kakoiTexObj->mTransform->Position(kakoiPos);
}

//シーンの決定
void ConfigScene::EnterScene(int num){
	bool isSpaceKey = Input::Trigger(KeyCode::Key_SPACE);
	bool isPad_B_Button = Input::Trigger(PAD_X_KeyCode::Button_B);
	if (isSpaceKey || isPad_B_Button) {
		SE(SoundManager::SoundSE_ID::Enum::Decision);
		std::string nextScenePass = GetScenePass(num);
		if (!m_fader) return;
		auto fader = m_fader->GetScript<Fader>();
		fader->OnSceneChnage(nextScenePass);
	}
}

//シーンのパスを取得
std::string ConfigScene::GetScenePass(int num){
	std::string sceneNamesPass[] = {
		m_volumeScenePass,
		m_howtoScenePass,
		m_comboScenePass,
		m_titleScenePass
	};
	return sceneNamesPass[num];
}

void ConfigScene::SE(SoundManager::SoundSE_ID::Enum seID){
	SoundManager::PlaySE(seID, XMVectorZero());
}

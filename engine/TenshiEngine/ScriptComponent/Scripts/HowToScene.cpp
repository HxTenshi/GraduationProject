#include "HowToScene.h"
#include "Fader.h"
#include "SoundManager.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void HowToScene::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void HowToScene::Start(){

}

//毎フレーム呼ばれます
void HowToScene::Update(){
	if (m_fader->GetScript<Fader>()->IsFader())return;
	bool isSpaceKey = Input::Trigger(KeyCode::Key_SPACE);
	bool isPad_B_Button = Input::Trigger(PAD_X_KeyCode::Button_B);
	if (isSpaceKey || isPad_B_Button) {
		SoundManager::PlaySE_2D(SoundManager::SoundSE_ID::Enum::Decision);
		if (!m_fader) return;
		auto fader = m_fader->GetScript<Fader>();
		fader->OnSceneChnage(configPass);
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void HowToScene::Finish(){

}

//コライダーとのヒット時に呼ばれます
void HowToScene::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void HowToScene::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void HowToScene::OnCollideExit(GameObject target){
	(void)target;
}
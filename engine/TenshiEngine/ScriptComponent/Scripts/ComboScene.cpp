#include "ComboScene.h"
#include "SoundManager.h"
#include "Fader.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void ComboScene::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ComboScene::Start(){

}

//毎フレーム呼ばれます
void ComboScene::Update(){
	bool isSpaceKey = Input::Trigger(KeyCode::Key_SPACE);
	bool isPad_B_Button = Input::Trigger(PAD_X_KeyCode::Button_B);
	if (isSpaceKey || isPad_B_Button) {
		SoundManager::PlaySE(SoundManager::SoundSE_ID::Enum::Decision,XMVectorZero());
		if (!m_fader) return;
		auto fader = m_fader->GetScript<Fader>();
		fader->OnSceneChnage(configPass);
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ComboScene::Finish(){
}

//コライダーとのヒット時に呼ばれます
void ComboScene::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void ComboScene::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void ComboScene::OnCollideExit(GameObject target){
	(void)target;
}
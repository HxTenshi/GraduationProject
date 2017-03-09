#include "ChangeScene.h"
#include "h_standard.h"
#include "Fader.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void ChangeScene::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ChangeScene::Start(){

}

//毎フレーム呼ばれます
void ChangeScene::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ChangeScene::Finish(){

}

//コライダーとのヒット時に呼ばれます
void ChangeScene::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void ChangeScene::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void ChangeScene::OnCollideExit(GameObject target){
	(void)target;
}

void ChangeScene::Next()
{
	Hx::System()->LockCursorPositionToWindowCenter(false);
	if (!m_Fade)return;
	auto fade = m_Fade->GetScript<Fader>();
	if (!fade)return;

	fade->OnSceneChnage(m_SceneName);
	//Hx::LoadScene(m_SceneName);
}

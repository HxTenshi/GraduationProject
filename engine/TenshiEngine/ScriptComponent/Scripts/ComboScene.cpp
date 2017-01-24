#include "ComboScene.h"
#include "SoundManager.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void ComboScene::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ComboScene::Start(){

}

//毎フレーム呼ばれます
void ComboScene::Update(){
	if (Input::Trigger(KeyCode::Key_SPACE)) {
		Hx::LoadScene(configPass);
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
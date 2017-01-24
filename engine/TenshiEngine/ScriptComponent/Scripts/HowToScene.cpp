#include "HowToScene.h"



//生成時に呼ばれます（エディター中も呼ばれます）
void HowToScene::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void HowToScene::Start(){

}

//毎フレーム呼ばれます
void HowToScene::Update(){
	if (Input::Trigger(KeyCode::Key_SPACE)) {
		Hx::LoadScene(configPass);
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
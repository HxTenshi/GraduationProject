#include "ForceShutdown.h"

#include "h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void ForceShutdown::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ForceShutdown::Start(){

}

//毎フレーム呼ばれます
void ForceShutdown::Update(){

	if (Input::Down(PAD_X_KeyCode::Button_START) && Input::Down(PAD_X_KeyCode::Button_BACK)) {
		Hx::Shutdown();
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ForceShutdown::Finish(){

}

//コライダーとのヒット時に呼ばれます
void ForceShutdown::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void ForceShutdown::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void ForceShutdown::OnCollideExit(GameObject target){
	(void)target;
}
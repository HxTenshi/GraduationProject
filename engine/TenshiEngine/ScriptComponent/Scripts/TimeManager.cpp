#include "TimeManager.h"
#include <h_standard.h>


//生成時に呼ばれます（エディター中も呼ばれます）
void TimeManager::Initialize(){
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void TimeManager::Start(){
	isSlow_ = false;
	scaleTime_ = 1.0f;
}

//毎フレーム呼ばれます
void TimeManager::Update(){
	
	if (Input::Down(KeyCode::Key_Z)) {
		isSlow_ = true;
	}
	if (Input::Down(KeyCode::Key_X)) {
		isSlow_ = false;
	}
	//デルタタイムを取得
	float deltaTime = Hx::DeltaTime()->GetNoScaleDeltaTime();

	//何秒必要とするか
	float temp = 1.0f / slowRate_;

	//スロー時の処理
	if (IsSlow()) 
		scaleTime_ -= temp * deltaTime;

	//通常時の処理
	else if(!IsSlow())
		scaleTime_ += temp* deltaTime;

	//0.1～1.0fに収まるようにクランプ
	scaleTime_ = min(max(0.1f,scaleTime_), 1.0f);
	//値を入れる
	Hx::DeltaTime()->SetTimeScale(scaleTime_);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void TimeManager::Finish(){

}

void TimeManager::OnSlow() {
	isSlow_ = true;
}

bool TimeManager::IsSlow() {
	return isSlow_;
}

//コライダーとのヒット時に呼ばれます
void TimeManager::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void TimeManager::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void TimeManager::OnCollideExit(GameObject target){
	(void)target;
}
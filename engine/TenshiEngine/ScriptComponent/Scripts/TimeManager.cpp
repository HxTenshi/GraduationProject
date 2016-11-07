#include "TimeManager.h"
#include <h_standard.h>


//生成時に呼ばれます（エディター中も呼ばれます）
void TimeManager::Initialize(){
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void TimeManager::Start(){
	m_isSlow = false;
	m_scaleTime = 1.0f;
}

//毎フレーム呼ばれます
void TimeManager::Update(){
	
	if (Input::Down(KeyCode::Key_Z)) {
		m_isSlow = true;
	}
	if (Input::Down(KeyCode::Key_X)) {
		m_isSlow = false;
	}
	//デルタタイムを取得
	float deltaTime = Hx::DeltaTime()->GetNoScaleDeltaTime();

	//何秒必要とするか
	float temp = 1.0f / m_slowRate;

	//スロー時の処理
	if (IsSlow()) 
		m_scaleTime -= temp * deltaTime;

	//通常時の処理
	else if(!IsSlow())
		m_scaleTime += temp* deltaTime;

	//0.1～1.0fに収まるようにクランプ
	m_scaleTime = min(max(0.1f,m_scaleTime), 1.0f);
	//値を入れる
	Hx::DeltaTime()->SetTimeScale(m_scaleTime);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void TimeManager::Finish(){

}

void TimeManager::OnSlow() {
	m_isSlow = true;
}

void TimeManager::OffSlow(){
	m_isSlow = false;
}

bool TimeManager::IsSlow() {
	return m_isSlow;
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
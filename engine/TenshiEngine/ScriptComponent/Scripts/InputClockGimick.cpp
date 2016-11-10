#include "InputClockGimick.h"
#include "h_standard.h"
#include "OutputGimic.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void InputClockGimick::Initialize(){
	m_TimeCounter = 0.0f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void InputClockGimick::Start(){

}

//毎フレーム呼ばれます
void InputClockGimick::Update(){
	if (m_Timer <= 0.0f)return;
	if (!m_Output)return;

	auto scr = OutputGimic::GetOutputGimic(m_Output);
	if (!scr)return;

	m_TimeCounter += Hx::DeltaTime()->GetDeltaTime();
	for(int i= 0;i<12;i++){
		if(m_TimeCounter >= m_Timer){
			scr->OnStart(gameObject);
			m_TimeCounter -= m_Timer;
		}
		else {
			break;
		}
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void InputClockGimick::Finish(){

}

//コライダーとのヒット時に呼ばれます
void InputClockGimick::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void InputClockGimick::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void InputClockGimick::OnCollideExit(GameObject target){
	(void)target;
}
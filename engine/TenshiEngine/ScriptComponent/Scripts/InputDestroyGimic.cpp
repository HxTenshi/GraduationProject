#include "InputDestroyGimic.h"
#include "OutputGimic.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void InputDestroyGimic::Initialize(){
	m_End = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void InputDestroyGimic::Start(){

}

//毎フレーム呼ばれます
void InputDestroyGimic::Update(){

	if (m_End)return;
	if (!m_OutputGimic)return;
	if (m_Target)return;

	auto scr = OutputGimic::GetOutputGimic(m_OutputGimic);
	if (!scr)return;

	scr->OnStart(gameObject);
	m_End = true;
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void InputDestroyGimic::Finish(){

}

//コライダーとのヒット時に呼ばれます
void InputDestroyGimic::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void InputDestroyGimic::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void InputDestroyGimic::OnCollideExit(GameObject target){
	(void)target;
}
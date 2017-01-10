#include "InputEnterObjectGimic.h"

#include "OutputGimic.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void InputEnterObjectGimic::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void InputEnterObjectGimic::Start(){

}

//毎フレーム呼ばれます
void InputEnterObjectGimic::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void InputEnterObjectGimic::Finish(){
	
}

#include "h_standard.h"
//コライダーとのヒット時に呼ばれます
void InputEnterObjectGimic::OnCollideBegin(GameObject target){

	if (!target)return;
	if (!_m_OutputGimic)return;

	auto gimic = OutputGimic::GetOutputGimic(_m_OutputGimic);
	if (!gimic)return;

	if (m_TargetObj) {
		if (target == m_TargetObj) {

			gimic->OnStart(gameObject);
			return;
		}
	}
	if (!m_TargetName.empty()) {

		if (target->Name() == m_TargetName) {

			gimic->OnStart(gameObject);
			return;
		}
	}
	//if (m_TargetLayer) {
		if (target->GetLayer() == m_TargetLayer) {

			gimic->OnStart(gameObject);
			return;
		}
	//}
	//if (m_TargetScript.empty()) {
	//	if (target->GetScript<>() == m_TargetScript) {

	//		return;
	//	}
	//}
}

//コライダーとのヒット中に呼ばれます
void InputEnterObjectGimic::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void InputEnterObjectGimic::OnCollideExit(GameObject target){
	(void)target;
}
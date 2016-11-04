#include "OutputStartGimic.h"
#include "Matinee.h"
#include "h_standard.h"
//生成時に呼ばれます（エディター中も呼ばれます）
void OutputStartGimic::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void OutputStartGimic::Start(){

}

//毎フレーム呼ばれます
void OutputStartGimic::Update(){

	if (Input::Trigger(KeyCode::Key_L)) {
		OnStart();
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void OutputStartGimic::Finish(){

}

//コライダーとのヒット時に呼ばれます
void OutputStartGimic::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void OutputStartGimic::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void OutputStartGimic::OnCollideExit(GameObject target){
	(void)target;
}

bool OutputStartGimic::OnStart(){
	if (!m_Output)return false;

	if (m_StartMatineeMode) {
		auto scr = m_Output->GetScript<Matinee>();
		if (!scr)return false;

		scr->Play();

		return scr->IsPlay();
	}
}

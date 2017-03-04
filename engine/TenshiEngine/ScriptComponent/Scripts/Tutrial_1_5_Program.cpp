#include "Tutrial_1_5_Program.h"
#include "UniqueObject.h"
#include "PlayerController.h"
#include "OutputGimic.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void Tutrial_1_5_Program::Initialize(){
	m_ComboClear = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Tutrial_1_5_Program::Start(){
	UniqueObject::GetPlayer()->GetScript<PlayerController>()->ClearCombo();
}

//毎フレーム呼ばれます
void Tutrial_1_5_Program::Update(){
	if (UniqueObject::GetPlayer()->GetScript<PlayerController>()->GetHitComboCount() > 20.0f && !m_ComboClear) {
		auto sender = OutputGimic::GetOutputGimic(m_HintDraw);
		if (sender)sender->OnStart(gameObject);
		m_ComboClear = true;
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Tutrial_1_5_Program::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Tutrial_1_5_Program::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void Tutrial_1_5_Program::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void Tutrial_1_5_Program::OnCollideExit(GameObject target){
	(void)target;
}
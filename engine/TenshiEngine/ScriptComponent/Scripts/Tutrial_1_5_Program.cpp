#include "Tutrial_1_5_Program.h"
#include "UniqueObject.h"
#include "PlayerController.h"
#include "OutputGimic.h"
#include "hintDraw.h"
#include "EnemyManager.h"
//生成時に呼ばれます（エディター中も呼ばれます）
void Tutrial_1_5_Program::Initialize(){
	m_ComboClear = false;
	m_Count = 0.0f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Tutrial_1_5_Program::Start(){
	UniqueObject::GetPlayer()->GetScript<PlayerController>()->ClearCombo();
	EnemyManager::EnemyAllDelete();
}

//毎フレーム呼ばれます
void Tutrial_1_5_Program::Update(){
	if (UniqueObject::GetPlayer()->GetScript<PlayerController>()->GetHitComboCount() >= 20 && !m_ComboClear) {
		m_ComboTextureObject = Hx::Instance(m_ComboTexture);
		m_ComboClear = true;
	}

	if (m_ComboClear) {
		m_Count += Hx::DeltaTime()->GetDeltaTime();
	}

	if (m_Count > m_Time) {
		Hx::DestroyObject(m_ComboTextureObject);
		hintDraw::OnStart_(gameObject);
		gameObject->RemoveComponent<ScriptComponent>();
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
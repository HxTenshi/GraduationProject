#include "Tutrial_1_5_Program.h"
#include "UniqueObject.h"
#include "PlayerController.h"
#include "OutputGimic.h"
#include "hintDraw.h"
#include "EnemyManager.h"
//生成時に呼ばれます（エディター中も呼ばれます）
void Tutrial_1_5_Program::Initialize(){
	m_ComboClear = false;
	m_GageMax = false;
	m_ComboBottonFlag = false;
	m_SpecialBottonFlag = false;
	m_Count = 0.0f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Tutrial_1_5_Program::Start(){
	UniqueObject::GetPlayer()->GetScript<PlayerController>()->ClearCombo();
	UniqueObject::GetPlayer()->GetScript<PlayerController>()->SetSpecial(0);
	EnemyManager::EnemyAllDelete();
	m_ComboTextureObject = Hx::Instance(m_ComboTexture);

}

//毎フレーム呼ばれます
void Tutrial_1_5_Program::Update(){
	
	m_Count += Hx::DeltaTime()->GetDeltaTime();	

	if (m_Count > m_Time && !m_ComboClear) {
		m_Count = 0;
		Hx::DestroyObject(m_ComboTextureObject);

		Hx::Debug()->Log(hintDraw::OnStart_(gameObject) ? "true" : "false");
		m_ComboClear = true;
	}

	if (!m_ComboBottonFlag && !m_SpecialBottonFlag && m_Count >= 2.5f && m_ComboClear) {
		m_ComboBottonFlag = true;
		for (auto i : m_ComboBottonPref) {
			m_ComboBotton.push_back(Hx::Instance(i));
		}
	}

	if (m_ComboBottonFlag && !m_SpecialBottonFlag && m_Count >= 2.5f && m_GageMax) {
		m_SpecialBottonFlag = true;
		m_SpercialBotton = Hx::Instance(m_SpercialBottonPref);
	}

	if (UniqueObject::GetPlayer()->GetScript<PlayerController>()->GetSpecial() >= 100 && !m_GageMax) {
		hintDraw::OnStart_(gameObject);
		for(auto i : m_ComboBotton)
			Hx::DestroyObject(i);
		m_GageMax = true;
		m_Count = 0;
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

void Tutrial_1_5_Program::DestroySpecialBotton()
{
	Hx::DestroyObject(m_SpercialBotton);
}

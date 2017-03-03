#include "OutputTextureUI.h"
# include "h_standard.h"
#include "OutputGimic.h"
# include "Game\Component\MaterialComponent.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void OutputTextureUI::Initialize(){
	m_Timer = 0;
	m_FirstFrame = true;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void OutputTextureUI::Start(){

}

//毎フレーム呼ばれます
void OutputTextureUI::Update(){
	if (m_FirstFrame) {

	}
	m_FirstFrame = false;
	m_Timer += Hx::DeltaTime()->GetDeltaTime();
	if (m_Timer >= m_WaitTime) {
		auto mc = gameObject->GetComponent<MaterialComponent>();
		if (!mc)return;
		auto color = mc->mAlbedo;
		color.w -= m_TimeScale * Hx::DeltaTime()->GetDeltaTime();
		bool end = false;
		if (color.w <= 0.0f) {
			color.w = 0.0f;
			end = true;
		}
		mc->SetAlbedoColor(color);
		if (end) {
			if (!m_EndAction)return;
			auto sender = OutputGimic::GetOutputGimic(m_EndAction);
			if (sender)sender->OnStart(gameObject);
			Hx::DestroyObject(gameObject);
		}
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void OutputTextureUI::Finish(){

}

//コライダーとのヒット時に呼ばれます
void OutputTextureUI::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void OutputTextureUI::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void OutputTextureUI::OnCollideExit(GameObject target){
	(void)target;
}
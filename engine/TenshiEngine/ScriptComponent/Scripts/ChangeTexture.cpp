#include "ChangeTexture.h"
#include "../h_standard.h"
#include "../h_component.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void ChangeTexture::Initialize(){
	m_Count = 0.0f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ChangeTexture::Start(){
	m_NowNumber = 0;
	gameObject->GetComponent<MaterialComponent>()->GetMaterialPtr(0)->SetTexture(m_Textures[m_NowNumber]);
}

//毎フレーム呼ばれます
void ChangeTexture::Update(){
	m_Count += Hx::DeltaTime()->GetDeltaTime();
	if (m_Count > m_ChangeTime) {
		m_Count = 0;
		m_NowNumber = (m_NowNumber == 0 ? 1 : 0);
		gameObject->GetComponent<MaterialComponent>()->GetMaterialPtr(0)->SetTexture(m_Textures[m_NowNumber]);
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ChangeTexture::Finish(){

}

//コライダーとのヒット時に呼ばれます
void ChangeTexture::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void ChangeTexture::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void ChangeTexture::OnCollideExit(GameObject target){
	(void)target;
}
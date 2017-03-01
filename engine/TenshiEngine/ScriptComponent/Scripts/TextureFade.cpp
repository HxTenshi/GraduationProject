#include "TextureFade.h"
#include "../h_standard.h"
#include "../h_component.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void TextureFade::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void TextureFade::Start(){
	gameObject->GetComponent<MaterialComponent>()->GetMaterialPtr(0)->SetAlbedo(XMFLOAT4(1,1,1, m_Fade));
}

//毎フレーム呼ばれます
void TextureFade::Update(){
	m_StartTime += Hx::DeltaTime()->GetDeltaTime();
	if(m_StartTime >= 2.0f)
	m_Fade -= m_FadeSpeed * Hx::DeltaTime()->GetDeltaTime();
	if (m_Fade <= 0.0f)Hx::DestroyObject(gameObject);
	gameObject->GetComponent<MaterialComponent>()->GetMaterialPtr(0)->SetAlbedo(XMFLOAT4(1, 1, 1, m_Fade));
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void TextureFade::Finish(){

}

//コライダーとのヒット時に呼ばれます
void TextureFade::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void TextureFade::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void TextureFade::OnCollideExit(GameObject target){
	(void)target;
}
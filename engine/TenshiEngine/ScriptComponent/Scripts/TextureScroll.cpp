#include "TextureScroll.h"

#include "h_component.h"
#include "h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void TextureScroll::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void TextureScroll::Start(){

}

//毎フレーム呼ばれます
void TextureScroll::Update(){

	auto mate = gameObject->GetComponent<MaterialComponent>();
	if (!mate)return;
	auto m = mate->GetMaterialPtr(0);
	if (!m)return;
	m->mOffset.x += m_Speed.x * Hx::DeltaTime()->GetDeltaTime();
	m->mOffset.y += m_Speed.y * Hx::DeltaTime()->GetDeltaTime();
	m->ParamUpdate();

	//auto m = mate->GetMaterial(0);
	//mate->SetMaterial(0,m);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void TextureScroll::Finish(){

}

//コライダーとのヒット時に呼ばれます
void TextureScroll::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void TextureScroll::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void TextureScroll::OnCollideExit(GameObject target){
	(void)target;
}
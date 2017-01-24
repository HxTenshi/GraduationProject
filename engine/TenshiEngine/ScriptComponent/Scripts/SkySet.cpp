#include "SkySet.h"

#include "h_component.h"

#include "UniqueObject.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void SkySet::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void SkySet::Start(){

}

//毎フレーム呼ばれます
void SkySet::Update(){
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void SkySet::Finish(){

}

//コライダーとのヒット時に呼ばれます
void SkySet::OnCollideBegin(GameObject target){
	
	if (UniqueObject::IsPlayer(target)) {
		if (auto obj = UniqueObject::GetCamera()) {
			if (auto cam = obj->GetComponent<CameraComponent>()) {
				cam->SetSkyTexture(m_SkyTexture);
			}
		}
	}
}

//コライダーとのヒット中に呼ばれます
void SkySet::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void SkySet::OnCollideExit(GameObject target){
	(void)target;
}
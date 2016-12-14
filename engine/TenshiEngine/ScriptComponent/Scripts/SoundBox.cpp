#include "SoundBox.h"
#include "../h_component.h"
#include "../h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void SoundBox::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void SoundBox::Start(){

}

//毎フレーム呼ばれます
void SoundBox::Update(){
	auto sound = gameObject->GetComponent<SoundComponent>();
	if (!sound->IsPlay()) {
		Hx::Debug()->Log("isplay");
		Hx::DestroyObject(gameObject);
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void SoundBox::Finish(){

}

//コライダーとのヒット時に呼ばれます
void SoundBox::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void SoundBox::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void SoundBox::OnCollideExit(GameObject target){
	(void)target;
}
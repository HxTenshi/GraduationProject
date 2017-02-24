#include "WeaponCreatePoint.h"
#include <random>

//生成時に呼ばれます（エディター中も呼ばれます）
void WeaponCreatePoint::Initialize(){
	
		auto g=Hx::Instance(weapon);
		g->mTransform->SetParent(gameObject);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WeaponCreatePoint::Start(){

}

//毎フレーム呼ばれます
void WeaponCreatePoint::Update(){

}z	

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void WeaponCreatePoint::Finish(){

}

//コライダーとのヒット時に呼ばれます
void WeaponCreatePoint::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void WeaponCreatePoint::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void WeaponCreatePoint::OnCollideExit(GameObject target){
	(void)target;
}
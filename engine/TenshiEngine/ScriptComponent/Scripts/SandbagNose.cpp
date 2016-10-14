#include "SandbagNose.h"
#include "Sandbag.h"
#include "../h_standard.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void SandbagNose::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void SandbagNose::Start(){

}

//毎フレーム呼ばれます
void SandbagNose::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void SandbagNose::Finish(){

}

//コライダーとのヒット時に呼ばれます
void SandbagNose::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void SandbagNose::OnCollideEnter(GameObject target){
	(void)target;
	if (target->GetLayer() == 4) {
		gameObject->mTransform->GetParent()->mTransform->GetParent()->GetScript<Sandbag>()->SetHitWall(true);
	}
}

//コライダーとのロスト時に呼ばれます
void SandbagNose::OnCollideExit(GameObject target){
	(void)target;
}
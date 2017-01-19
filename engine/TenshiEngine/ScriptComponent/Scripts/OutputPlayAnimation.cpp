#include "OutputPlayAnimation.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void OutputPlayAnimation::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void OutputPlayAnimation::Start(){

}

//毎フレーム呼ばれます
void OutputPlayAnimation::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void OutputPlayAnimation::Finish(){

}

//コライダーとのヒット時に呼ばれます
void OutputPlayAnimation::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void OutputPlayAnimation::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void OutputPlayAnimation::OnCollideExit(GameObject target){
	(void)target;
}
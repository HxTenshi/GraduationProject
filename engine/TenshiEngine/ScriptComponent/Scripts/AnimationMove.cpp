#include "AnimationMove.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void AnimationMove::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void AnimationMove::Start(){

}

//毎フレーム呼ばれます
void AnimationMove::Update(){
	if (m_MoveObject && m_BoneObject) {
		auto model = m_MoveObject->mTransform->WorldPosition();
		auto bone = m_BoneObject->mTransform->WorldPosition();
		model.y = 0.0f;
		bone.y = 0.0f;
		bone -= model;
		float z = XMVector3Length(bone).x;
		m_MoveObject->mTransform->Position(XMVectorSet(0,0,-z,1));
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void AnimationMove::Finish(){

}

//コライダーとのヒット時に呼ばれます
void AnimationMove::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void AnimationMove::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void AnimationMove::OnCollideExit(GameObject target){
	(void)target;
}
#include "AnimationMove.h"
#include"UniqueObject.h"
#include "PlayerController.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void AnimationMove::Initialize(){
	m_BackPos = XMVectorZero();
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void AnimationMove::Start(){

}

//毎フレーム呼ばれます
void AnimationMove::Update(){

	if (m_MoveObject && m_BoneObject) {
		auto model = m_MoveObject->mTransform->WorldPosition();
		auto bone = m_BoneObject->mTransform->WorldPosition();
		if(!m_Center){
			model.y = 0.0f;
			bone.y = 0.0f;
			bone -= model;
			float z = XMVector3Length(bone).x;
			m_MoveObject->mTransform->Position(XMVectorSet(0, 0, -z, 1));
		}
		else if(m_ZeroSetting){
			auto s = m_MoveObject->mTransform->Scale();
			m_MoveObject->mTransform->Position(m_BoneObject->mTransform->Position()*s);
		}
		else {
			bool flag = false;
			if (XMVector3Length(m_BoneObject->mTransform->Position()).x > 0.1f) {
				flag = true;
			}
			bone -= model;
			m_MoveObject->mTransform->Position(-bone);
			if(flag){
				auto move = bone - m_BackPos;

				if (auto p = UniqueObject::GetPlayer()) {
					if (auto pc = p->GetScript<PlayerController>()) {
						pc->AddMove(move);
					}
				}

			}

			m_BackPos = bone;
		}
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
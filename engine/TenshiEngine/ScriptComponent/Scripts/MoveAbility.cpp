#include "MoveAbility.h"
#include "../h_standard.h"
#include "../h_component.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void MoveAbility::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void MoveAbility::Start(){
	onMove = false;
}

//毎フレーム呼ばれます
void MoveAbility::Update(){

	if (onMove == true && mMovePoint)
	{
		XMVECTOR targetPosition = mMovePoint->mTransform->WorldPosition();
		mTime += Hx::DeltaTime()->GetDeltaTime() * mSpeed;
		if (mTime < 0.0f) {
			mTime = 0.0f;
		}
		else if (mTime > 1.0f) {
			mTime = 1.0f;
			onMove = false;
		}
		XMVECTOR resultPosition = mMoveActor->mTransform->WorldPosition() * (1.0f - mTime) + targetPosition * mTime;
		mMoveActor->mTransform->WorldPosition(resultPosition);
		//mCC->Move(resultPosition);
	}

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void MoveAbility::Finish(){

}

//コライダーとのヒット時に呼ばれます
void MoveAbility::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void MoveAbility::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void MoveAbility::OnCollideExit(GameObject target){
	(void)target;
}

void MoveAbility::SetPoint(GameObject target, weak_ptr<CharacterControllerComponent> cc)
{
	mCC = cc;
	mMovePoint = target;
}

void MoveAbility::OnMove()
{
	mMoveActor = Hx::FindActor("キャラクター");
	mTime;
	onMove = true;
}

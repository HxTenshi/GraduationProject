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
		//ここの1はキャラのYのサイズにしないと・・・。
		targetPosition += XMVectorSet(0,1.0f,0,0);
		mTime += Hx::DeltaTime()->GetDeltaTime() * mSpeed;
		if (mTime < 0.0f) {
			mTime = 0.0f;
		}
		else if (mTime > 1.0f) {
			mTime = 1.0f;
			onMove = false;
			Hx::Debug()->Log("移動完了しているはず");
		}

		Hx::Debug()->Log("移動中");
		
		XMVECTOR resultPosition = mMoveStartPosition * (1.0f - mTime) + targetPosition * mTime;
		resultPosition = XMVectorLerp(mMoveStartPosition, targetPosition, mTime);

		
		
		mCC->Teleport(resultPosition);
	}

	//Qボタンを長く押して下記のように
	//カメラと自分の位置を引いてカメラの方向を出して飛ばす。

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
	//ここの1はキャラのYのサイズにしないと・・・。
	mMoveStartPosition = mMoveActor->mTransform->WorldPosition();
	mMoveStartPosition += XMVectorSet(0, 1.0f, 0, 0);
	mTime;
	onMove = true;
}

#include "MoveAbility.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "PlayerController.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void MoveAbility::Initialize(){
	mPC = NULL;
	mDown = 0;
	mDownTime = 0.0f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void MoveAbility::Start(){
	onMove = false;
}

//毎フレーム呼ばれます
void MoveAbility::Update(){

	//投げたところへの移動
	if (onMove == true && mMovePoint && mPC)
	{
		XMVECTOR targetPosition = mMovePoint->mTransform->WorldPosition();
		//ここの1はキャラのYのサイズにしないと・・・。
		//XMVECTOR characterSize = XMVectorSet(0, 3, 0, 0);
		//targetPosition += characterSize;

		////mTime += Hx::DeltaTime()->GetDeltaTime() * mSpeed;
		//if (mTime < 0.0f) {
		//	mTime = 0.0f;
		//}
		//else if (mTime > 1.0f) {
		//	mTime = 1.0f;
		//	onMove = false;
		//	Hx::Debug()->Log("移動完了しているはず。");
		//	Hx::Debug()->Log("ちょっと回転しながら飛んでいきます");
		//}
		//
		//XMVECTOR resultPosition = mMoveStartPosition * (1.0f - mTime) + targetPosition * mTime;
		//
		////対象との距離を少し離す
		//float distance = 2.0f;
		//characterSize = targetPosition - mMoveStartPosition;
		//characterSize = XMVector3Normalize(characterSize);
		//resultPosition = resultPosition - characterSize * distance * mTime;


		if (OnTargetDistance(targetPosition)) {
			onMove = false;
				mPC->SpeedJumpWeaponCatch(mMovePoint);
		}
		else {
			mMoveActor = gameObject->mTransform->GetParent();
				mPC->SpeedJump(XMVector3Normalize(mMovePoint->mTransform->WorldPosition() - mMoveActor->mTransform->WorldPosition()) * mSpeed);
		}

		if (mDown == 0) {
			mDownTime = 0.0f;
		}
		mDown = 0;

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

	KnockBack(target);

}

//コライダーとのロスト時に呼ばれます
void MoveAbility::OnCollideExit(GameObject target){
	(void)target;
}

void MoveAbility::SetPoint(GameObject target, PlayerController* pc)
{
	mPC = pc;
	mMovePoint = target;
}

void MoveAbility::OnMove()
{
	mMoveActor = gameObject->mTransform->GetParent();
	//ここの1はキャラのYのサイズにしないと・・・。
	mMoveStartPosition = mMoveActor->mTransform->WorldPosition();
	mMoveStartPosition += XMVectorSet(0, 1.3f, 0, 0);
	mTime = 0;
	mDown = 0;
	mDownTime = 0.0f;
	onMove = true;
	if(mPC){
		mPC->SpeedJump(XMVector3Normalize(mMovePoint->mTransform->WorldPosition() - mMoveActor->mTransform->WorldPosition()) * mSpeed);
	}
}

//敵に当たった際のノックバック処理
void MoveAbility::KnockBack(GameObject target)
{
	if (!onMove)return;

	mDown = 1;
	if (mDownTime < 0.3f) {
		mDownTime += Hx::DeltaTime()->GetDeltaTime();
		return;
	}

	if (target->GetLayer() == 3) {
		//Hx::Debug()->Log("移動中に敵に当たってしまった。");
		mTime = 0;
		onMove = false;
		if (mPC) {
			auto v = XMVector3Normalize(mMovePoint->mTransform->WorldPosition() - mMoveActor->mTransform->WorldPosition()) * -1;
			mPC->Damage(0.0f, v, PlayerController::KnockBack::Down, true, true);
		}
	}
	if (target->GetLayer() == 4) {
		//Hx::Debug()->Log("移動中にステージに当たってしまった。");
		mTime = 0;
		onMove = false;
		if (mPC) {
			auto v = XMVector3Normalize(mMovePoint->mTransform->WorldPosition() - mMoveActor->mTransform->WorldPosition()) * -1;
			mPC->Damage(0.0f, v, PlayerController::KnockBack::Down, true, true);
		}
	}
	{
		//Hx::Debug()->Log("何かに当たってしまった。");
		mTime = 0;
		onMove = false;
		if (mPC) {
			auto v = XMVector3Normalize(mMovePoint->mTransform->WorldPosition() - mMoveActor->mTransform->WorldPosition()) * -1;
			mPC->Damage(0.0f, v, PlayerController::KnockBack::Down, true, true);
		}
	}
}

bool MoveAbility::OnTargetDistance(XMVECTOR target)
{
	float distance = XMVector3Length(mMoveActor->mTransform->WorldPosition() - target).x;
	if (distance < 2)return true;
	return false;
}
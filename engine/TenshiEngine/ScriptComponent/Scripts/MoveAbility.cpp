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

	//投げたところへの移動
	if (onMove == true && mMovePoint)
	{
		XMVECTOR targetPosition = mMovePoint->mTransform->WorldPosition();
		//ここの1はキャラのYのサイズにしないと・・・。
		XMVECTOR characterSize = XMVectorSet(0, 3, 0, 0);
		targetPosition += characterSize;

		mTime += Hx::DeltaTime()->GetDeltaTime() * mSpeed;
		if (mTime < 0.0f) {
			mTime = 0.0f;
		}
		else if (mTime > 1.0f) {
			mTime = 1.0f;
			onMove = false;
			Hx::Debug()->Log("移動完了しているはず。");
			Hx::Debug()->Log("ちょっと回転しながら飛んでいきます");
		}
		
		XMVECTOR resultPosition = mMoveStartPosition * (1.0f - mTime) + targetPosition * mTime;
		
		//対象との距離を少し離す
		float distance = 2.0f;
		characterSize = targetPosition - mMoveStartPosition;
		characterSize = XMVector3Normalize(characterSize);
		resultPosition = resultPosition - characterSize * distance * mTime;// *mTime;
		//resultPosition = XMVectorLerp(mMoveStartPosition, targetPosition, mTime);

		//XMVECTOR rote = mCC->gameObject->mTransform->DegreeRotate() + XMVectorSet(65 - 65 * mTime, -45 + 45 * mTime, 0, 0);
		//mCC->gameObject->mTransform->DegreeRotate(rote);
		
		mCC->Teleport(resultPosition);
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
	mMoveActor = gameObject->mTransform->GetParent();
	//ここの1はキャラのYのサイズにしないと・・・。
	mMoveStartPosition = mMoveActor->mTransform->WorldPosition();
	mMoveStartPosition += XMVectorSet(0, 1.3f, 0, 0);
	mTime = 0;
	onMove = true;
}

//敵に当たった際のノックバック処理
void MoveAbility::KnockBack(std::function<void(void)> function,GameObject target)
{
	if (!onMove)return;
	if (target->GetLayer() == 3) {
		Hx::Debug()->Log("移動中に敵に当たってしまった。");
		function();
		mTime = 0;
		onMove = false;
	}
}

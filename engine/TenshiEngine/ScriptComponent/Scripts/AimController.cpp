#include "AimController.h"
#include "h_component.h"
#include "h_standard.h"
# include "SoundManager.h"
//生成時に呼ばれます（エディター中も呼ばれます）
void AimController::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void AimController::Start(){
	if (mTargetUI)mTargetUI->Disable();
	SoundManager::PlayBGM(SoundManager::SoundBGM_ID::GamePlay);
}

//毎フレーム呼ばれます
void AimController::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void AimController::Finish(){

}

//コライダーとのヒット時に呼ばれます
void AimController::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void AimController::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void AimController::OnCollideExit(GameObject target){
	(void)target;
}

void AimController::ChangeAimMode(TPSCamera* camera,GameObject player, bool isFree)
{
	//960 540, 930 420
	if (isFree)
	{
		camera->SetDistance(mAimPoint.z);//0.5f
		camera->SetUp(mAimPoint.y);//2.5f
		camera->SetLeft(mAimPoint.x);//2.5f
		camera->SetSpringDamping(mDamping);
		camera->SetSpringStiffness(mStiffness);
		if (mTargetUI) {
			mTargetUI->Enable();

			//if (camera->GetLookTarget()) {
			//	mTargetUI->mTransform->Position(XMVectorSet(960.0f, 540.0f, 0.0f, 1.0f));
			//}
			//else {
			//	mTargetUI->mTransform->Position(XMVectorSet(930.0f, 420.0f, 0.0f, 1.0f));
			//}
		}
	}
	else
	{
		camera->SetDistance(mDefoultPoint.z);//0.5f
		camera->SetUp(mDefoultPoint.y);//2.5f
		camera->SetLeft(mDefoultPoint.x);//2.5f
		camera->SetSpringDamping(mDefoultDamping);
		camera->SetSpringStiffness(mDefoultStiffness);
		if (mTargetUI)mTargetUI->Disable();
	}
	PlayerRotate(camera, player);
}

void AimController::PlayerRotate(TPSCamera * camera, GameObject player)
{
	//XMVECTOR angle = XMVectorSet(0,0,0,0);
	//angle = camera->gameObject->mTransform->DegreeRotate();
	//player->mTransform->DegreeRotate(angle);
}

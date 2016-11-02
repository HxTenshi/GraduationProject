#include "AimController.h"
#include "h_component.h"
#include "h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void AimController::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void AimController::Start(){

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
	//Hx::Debug()->Log("スプリング : "+std::to_string(camera->GetSpringDamping()));
	//Hx::Debug()->Log("スプリングStiffness : " + std::to_string(camera->GetSpringStiffness()));

	if (isFree)
	{
		camera->SetDistance(mAimPoint.z);//0.5f
		camera->SetUp(mAimPoint.y);//2.5f
		camera->SetLeft(mAimPoint.x);//2.5f
		camera->SetSpringDamping(mDamping);
		camera->SetSpringStiffness(mStiffness);
	}
	else
	{
		camera->SetDistance(mDefoultPoint.z);//0.5f
		camera->SetUp(mDefoultPoint.y);//2.5f
		camera->SetLeft(mDefoultPoint.x);//2.5f
		camera->SetSpringDamping(mDefoultDamping);
		camera->SetSpringStiffness(mDefoultStiffness);
	}
	PlayerRotate(camera, player);
}

void AimController::PlayerRotate(TPSCamera * camera, GameObject player)
{
	XMVECTOR angle = XMVectorSet(0,0,0,0);
	angle = camera->gameObject->mTransform->DegreeRotate();
	player->mTransform->DegreeRotate(angle);
}

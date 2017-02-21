#include "AimController.h"
#include "h_component.h"
#include "h_standard.h"
# include "SoundManager.h"
//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void AimController::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void AimController::Start(){
	if (mUIParent) {
		for (auto i : mUIParent->mTransform->Children()) {
			if (i->Name() == "TargetUI")mTargetUI = i;
		}
	}
	SoundManager::PlayBGM(SoundManager::SoundBGM_ID::GamePlay);
}

//���t���[���Ă΂�܂�
void AimController::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void AimController::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void AimController::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void AimController::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void AimController::OnCollideExit(GameObject target){
	(void)target;
}

void AimController::ChangeAimMode(TPSCamera* camera,GameObject player, bool isFree)
{
	if (isFree)
	{
		camera->SetDistance(mAimPoint.z);//0.5f
		camera->SetUp(mAimPoint.y);//2.5f
		camera->SetLeft(mAimPoint.x);//2.5f
		camera->SetSpringDamping(mDamping);
		camera->SetSpringStiffness(mStiffness);
		if (mTargetUI)mTargetUI->Enable();
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

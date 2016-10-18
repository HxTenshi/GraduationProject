#include "MoveAbility.h"
#include "../h_standard.h"
#include "../h_component.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void MoveAbility::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void MoveAbility::Start(){
	onMove = false;
}

//���t���[���Ă΂�܂�
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

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void MoveAbility::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void MoveAbility::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void MoveAbility::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
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
	mMoveActor = Hx::FindActor("�L�����N�^�[");
	mTime;
	onMove = true;
}

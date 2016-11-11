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

	//�������Ƃ���ւ̈ړ�
	if (onMove == true && mMovePoint)
	{
		XMVECTOR targetPosition = mMovePoint->mTransform->WorldPosition();
		//������1�̓L������Y�̃T�C�Y�ɂ��Ȃ��ƁE�E�E�B
		XMVECTOR characterSize = XMVectorSet(0, 3, 0, 0);
		targetPosition += characterSize;

		mTime += Hx::DeltaTime()->GetDeltaTime() * mSpeed;
		if (mTime < 0.0f) {
			mTime = 0.0f;
		}
		else if (mTime > 1.0f) {
			mTime = 1.0f;
			onMove = false;
			Hx::Debug()->Log("�ړ��������Ă���͂��B");
			Hx::Debug()->Log("������Ɖ�]���Ȃ�����ł����܂�");
		}
		
		XMVECTOR resultPosition = mMoveStartPosition * (1.0f - mTime) + targetPosition * mTime;
		
		//�ΏۂƂ̋�������������
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
	mMoveActor = gameObject->mTransform->GetParent();
	//������1�̓L������Y�̃T�C�Y�ɂ��Ȃ��ƁE�E�E�B
	mMoveStartPosition = mMoveActor->mTransform->WorldPosition();
	mMoveStartPosition += XMVectorSet(0, 1.3f, 0, 0);
	mTime = 0;
	onMove = true;
}

//�G�ɓ��������ۂ̃m�b�N�o�b�N����
void MoveAbility::KnockBack(std::function<void(void)> function,GameObject target)
{
	if (!onMove)return;
	if (target->GetLayer() == 3) {
		Hx::Debug()->Log("�ړ����ɓG�ɓ������Ă��܂����B");
		function();
		mTime = 0;
		onMove = false;
	}
}

#include "MoveAbility.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "PlayerController.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void MoveAbility::Initialize(){
	mPC = NULL;
	m_LastPos = XMVectorZero();
	m_MoveFirst = false;

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void MoveAbility::Start(){
	onMove = false;
}

//���t���[���Ă΂�܂�
void MoveAbility::Update(){

	//�������Ƃ���ւ̈ړ�
	if (onMove == true && mMovePoint && mPC)
	{
		XMVECTOR targetPosition = mMovePoint->mTransform->WorldPosition();
		//������1�̓L������Y�̃T�C�Y�ɂ��Ȃ��ƁE�E�E�B
		//XMVECTOR characterSize = XMVectorSet(0, 3, 0, 0);
		//targetPosition += characterSize;

		////mTime += Hx::DeltaTime()->GetDeltaTime() * mSpeed;
		//if (mTime < 0.0f) {
		//	mTime = 0.0f;
		//}
		//else if (mTime > 1.0f) {
		//	mTime = 1.0f;
		//	onMove = false;
		//	Hx::Debug()->Log("�ړ��������Ă���͂��B");
		//	Hx::Debug()->Log("������Ɖ�]���Ȃ�����ł����܂�");
		//}
		//
		//XMVECTOR resultPosition = mMoveStartPosition * (1.0f - mTime) + targetPosition * mTime;
		//
		////�ΏۂƂ̋�������������
		//float distance = 2.0f;
		//characterSize = targetPosition - mMoveStartPosition;
		//characterSize = XMVector3Normalize(characterSize);
		//resultPosition = resultPosition - characterSize * distance * mTime;


		if (OnTargetDistance(targetPosition)) {
			onMove = false;
				mPC->SpeedJumpWeaponCatch(mMovePoint);
		}
		else {
			auto temp = mMoveActor->mTransform->WorldPosition();
			if (XMVector3Length(m_LastPos - temp).x < mSpeed * Hx::DeltaTime()->GetDeltaTime() * 0.5f && !m_MoveFirst) {

				auto v = XMVector3Normalize(mMovePoint->mTransform->WorldPosition() - mMoveActor->mTransform->WorldPosition()) * -1;
				mPC->Damage(0.0f, v, PlayerController::KnockBack::Down, true, true);
				onMove = false;
				mTime = 0;
			}
			else {
				m_LastPos = temp;
				mMoveActor = gameObject->mTransform->GetParent();
				mPC->SpeedJump(XMVector3Normalize(mMovePoint->mTransform->WorldPosition() - temp) * mSpeed);
			}
			m_MoveFirst = false;
		}

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

	KnockBack(target);

}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
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
	//������1�̓L������Y�̃T�C�Y�ɂ��Ȃ��ƁE�E�E�B
	mMoveStartPosition = mMoveActor->mTransform->WorldPosition();
	mMoveStartPosition += XMVectorSet(0, 1.3f, 0, 0);
	mTime = 0;
	onMove = true;
	m_MoveFirst = true;
	m_LastPos = mMoveActor->mTransform->WorldPosition();
	if(mPC){
		mPC->SpeedJump(XMVector3Normalize(mMovePoint->mTransform->WorldPosition() - m_LastPos) * mSpeed);
	}
}

//�G�ɓ��������ۂ̃m�b�N�o�b�N����
void MoveAbility::KnockBack(GameObject target)
{
	if (!onMove)return;


	if (target->GetLayer() == 3) {
		//Hx::Debug()->Log("�ړ����ɓG�ɓ������Ă��܂����B");
		mTime = 0;
		onMove = false;
		if (mPC) {
			auto v = XMVector3Normalize(mMovePoint->mTransform->WorldPosition() - mMoveActor->mTransform->WorldPosition()) * -1;
			mPC->Damage(0.0f, v, PlayerController::KnockBack::Down, true, true);
		}
	}
	//if (target->GetLayer() == 4) {
	//	//Hx::Debug()->Log("�ړ����ɃX�e�[�W�ɓ������Ă��܂����B");
	//	mTime = 0;
	//	onMove = false;
	//	if (mPC) {
	//		auto v = XMVector3Normalize(mMovePoint->mTransform->WorldPosition() - mMoveActor->mTransform->WorldPosition()) * -1;
	//		mPC->Damage(0.0f, v, PlayerController::KnockBack::Down, true, true);
	//	}
	//}
	//{
	//	//Hx::Debug()->Log("�����ɓ������Ă��܂����B");
	//	mTime = 0;
	//	onMove = false;
	//	if (mPC) {
	//		auto v = XMVector3Normalize(mMovePoint->mTransform->WorldPosition() - mMoveActor->mTransform->WorldPosition()) * -1;
	//		mPC->Damage(0.0f, v, PlayerController::KnockBack::Down, true, true);
	//	}
	//}
}

bool MoveAbility::OnTargetDistance(XMVECTOR target)
{
	float distance = XMVector3Length(mMoveActor->mTransform->WorldPosition() - target).x;
	if (distance < 2)return true;
	return false;
}
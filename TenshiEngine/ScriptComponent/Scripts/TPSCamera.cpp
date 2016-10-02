#include "TPSCamera.h"

TPSCamera::TPSCamera(){
	mDistance = 5.0f;
	mUp = 1.0f;
	mTarget = NULL;
}

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TPSCamera::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TPSCamera::Start(){
}

//���t���[���Ă΂�܂�
void TPSCamera::Update(){

	if (mTarget){
		auto pos = mTarget->mTransform->WorldPosition();
		auto back = -mTarget->mTransform->Forward();
		pos.y += mUp;
		auto campos = pos + back * mDistance;
		gameObject->mTransform->WorldPosition(campos);
		if (XMVector3Equal(pos, campos)){

			pos += mTarget->mTransform->Forward();
			auto rotate = XMMatrixTranspose(XMMatrixLookAtLH(campos, pos, XMVectorSet(0, 1, 0, 1)));
			auto q = XMQuaternionRotationMatrix(rotate);
			gameObject->mTransform->WorldQuaternion(q);
		}
		else{
			auto rotate = XMMatrixTranspose(XMMatrixLookAtLH(campos, pos, XMVectorSet(0, 1, 0, 1)));
			auto q = XMQuaternionRotationMatrix(rotate);
			gameObject->mTransform->WorldQuaternion(q);
		}
	}

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void TPSCamera::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void TPSCamera::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void TPSCamera::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void TPSCamera::OnCollideExit(GameObject target){
	(void)target;
}
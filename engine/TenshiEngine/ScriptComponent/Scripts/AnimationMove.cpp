#include "AnimationMove.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void AnimationMove::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void AnimationMove::Start(){

}

//���t���[���Ă΂�܂�
void AnimationMove::Update(){
	if (m_MoveObject && m_BoneObject) {
		auto model = m_MoveObject->mTransform->WorldPosition();
		auto bone = m_BoneObject->mTransform->WorldPosition();
		model.y = 0.0f;
		bone.y = 0.0f;
		bone -= model;
		float z = XMVector3Length(bone).x;
		m_MoveObject->mTransform->Position(XMVectorSet(0,0,-z,1));
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void AnimationMove::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void AnimationMove::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void AnimationMove::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void AnimationMove::OnCollideExit(GameObject target){
	(void)target;
}
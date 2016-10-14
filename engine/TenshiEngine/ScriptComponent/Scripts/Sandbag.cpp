#include "Sandbag.h"

#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Sandbag::Initialize(){
	mGravity = XMVectorSet(0, -9.81f, 0, 1);
	angle = 0.0f;
	changeVec = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Sandbag::Start(){
}

//���t���[���Ă΂�܂�
void Sandbag::Update(){
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	auto vec = XMVectorZero();
	if (!changeVec) {
		auto forward = gameObject->mTransform->Forward();
		forward = XMVector3Normalize(forward);
		auto pos = gameObject->mTransform->Position();
		vec += forward * speed * Hx::DeltaTime()->GetDeltaTime();
	}
	else {

	}
	if (!cc->IsGround()) {
		vec += mGravity * Hx::DeltaTime()->GetDeltaTime();
	}
	cc->Move(vec);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Sandbag::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Sandbag::OnCollideBegin(GameObject target) {
	(void)target;	
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Sandbag::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Sandbag::OnCollideExit(GameObject target){
	(void)target;
}

void Sandbag::SetHitWall(bool hitWall)
{
	changeVec = true;
}

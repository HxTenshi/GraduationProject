#include "PlayerController.h"

#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PlayerController::Initialize(){
	mJump = XMVectorZero();
	mGravity = XMVectorSet(0, -9.81f, 0,1);
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PlayerController::Start(){
}

//���t���[���Ă΂�܂�
void PlayerController::Update(){

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	
	float time = game->DeltaTime()->GetDeltaTime();
	
	float speed = 10.0f;
	float x = 0, y = 0;
	if (Input::Down(KeyCoord::Key_W)){
		y = -1.0f;
	}
	if (Input::Down(KeyCoord::Key_S)){
		y = 1.0f;
	}
	if (Input::Down(KeyCoord::Key_D)){
		x = -1.0f;
	}
	if (Input::Down(KeyCoord::Key_A)){
		x = 1.0f;
	}
	
	
	
	if (cc->IsGround()){
		mJump = XMVectorZero();
		if (Input::Trigger(KeyCoord::Key_SPACE)){
			mJump.y += 3.5f;
		}
	}
	mJump += mGravity * time;
	
	auto xy = XMVector2Normalize(XMVectorSet(x, y, 0, 1)) * speed * time;
	auto p = XMVectorZero();
	p += xy.y * gameObject->mTransform->Forward();
	p += xy.x * gameObject->mTransform->Left();
	p += mJump * time;
	
	cc->Move(p);
	

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void PlayerController::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PlayerController::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PlayerController::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void PlayerController::OnCollideExit(Actor* target){
	(void)target;
}
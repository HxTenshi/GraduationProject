#include "PlayerController.h"

#include "../h_standard.h"
#include "../h_component.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PlayerController::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PlayerController::Start(){
}

//���t���[���Ă΂�܂�
void PlayerController::Update(){


	float speed = 10;
	float x = 0, y = 0;
	if(Input::Down(KeyCoord::Key_W)){
		y = 1.0f;
	}
	if (Input::Down(KeyCoord::Key_S)){
		y = -1.0f;
	}
	if (Input::Down(KeyCoord::Key_D)){
		x = -1.0f;
	}
	if (Input::Down(KeyCoord::Key_A)){
		x = 1.0f;
	}
	auto xy = XMVector2Normalize(XMVectorSet(x, y, 0, 1)) * speed * game->DeltaTime()->GetDeltaTime();
	auto pos = gameObject->mTransform->WorldPosition();
	pos += xy.y * gameObject->mTransform->Forward();
	pos += xy.x * gameObject->mTransform->Left();
	gameObject->mTransform->WorldPosition(pos);

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
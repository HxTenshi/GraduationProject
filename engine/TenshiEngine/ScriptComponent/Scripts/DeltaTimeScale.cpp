#include "DeltaTimeScale.h"
#include "Game/Script/IGame.h"
#include "Game/DeltaTime.h"

DeltaTimeScale::DeltaTimeScale(){
	mDeltaTimeScale = 1.0f;

}

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void DeltaTimeScale::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void DeltaTimeScale::Start(){

}

//���t���[���Ă΂�܂�
void DeltaTimeScale::Update(){
	Hx::DeltaTime()->SetTimeScale(mDeltaTimeScale);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void DeltaTimeScale::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void DeltaTimeScale::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void DeltaTimeScale::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void DeltaTimeScale::OnCollideExit(GameObject target){
	(void)target;
}
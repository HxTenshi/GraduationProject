#include "DemoWeapon.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void DemoWeapon::Initialize(){
	mShoot = false;
	mThrowVector = XMVectorSet(0, 0, 0, 0);
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void DemoWeapon::Start(){

}

//���t���[���Ă΂�܂�
void DemoWeapon::Update(){
	if(mShoot)gameObject->mTransform->WorldPosition(gameObject->mTransform->WorldPosition() + mThrowVector);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void DemoWeapon::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void DemoWeapon::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void DemoWeapon::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void DemoWeapon::OnCollideExit(GameObject target){
	(void)target;
}

void DemoWeapon::SetReleaseVector(GameObject player,XMVECTOR position)
{
	//�Ȃ����A�N�Z�X�G���[�ɂȂ�
	Hx::Debug()->Log(std::to_string(gameObject->mTransform->Position().x));

	//float speed = 10;
	//auto front = player->mTransform->Forward();
	//front = XMVector3Normalize(front);
	//XMVECTOR power = front * speed * Hx::DeltaTime()->GetDeltaTime();
	//XMVECTOR result = power + player->mTransform->Position();

	//mThrowVector = power * gameObject->mTransform->Position();
}

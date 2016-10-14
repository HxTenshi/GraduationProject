#include "Sandbag.h"

#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Sandbag::Initialize(){
	mGravity = XMVectorSet(0, -9.81f, 0, 1);
	angle = 0.0f;
	maxAngle = 0.0f;
	subAngle = 0.0f;
	walkReturnFlag = false;
	changeVec = false;
	damageFlag = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Sandbag::Start(){
}

//���t���[���Ă΂�܂�
void Sandbag::Update(){
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	//�ǂɓ������ďC���������܂��ǂɓ������Ă��邩���ׂ�
	if (walkReturnFlag) {
		changeVec = false;
		subAngle = 0.0f;
	}

	vec = XMVectorZero();
	if (!changeVec) {
		Walk();
	}
	else {
		WallHit();
	}

	//�d��
	if (!cc->IsGround()) {
		vec += mGravity;
	}

	//Angle�̃N�����v
	if (angle > 360)angle -= 360.0f;
	else if (angle < 0)angle += 360.0f;

	if (Input::Trigger(KeyCoord::Key_D)) {
		Damage(1);
	}

	//�l��Set
	gameObject->mTransform->Rotate(XMVectorSet(0, angle * 3.14f / 180.0f, 0, 0));
	cc->Move(vec  * Hx::DeltaTime()->GetDeltaTime());

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
	walkReturnFlag = false;
	if (!changeVec) {
		changeVec = true;
		int random = rand();
		maxAngle = rand() % 90;
		if (random % 2 == 0) {
			maxAngle *= -1;
		}
	}
}

void Sandbag::Damage(int damage)
{
	hp -= damage;
	vec += XMVectorSet(0, 500, 0, 0);
}

void Sandbag::Walk()
{
	auto forward = gameObject->mTransform->Forward();
	forward = XMVector3Normalize(forward);
	auto pos = gameObject->mTransform->Position();
	vec += forward * speed;
}

void Sandbag::WallHit()
{
	float rotateVec = rotateSpeed * Hx::DeltaTime()->GetDeltaTime();
	subAngle += rotateVec;
	if (maxAngle < 0.0f) {
		angle -= rotateVec;
	}
	else {
		angle += rotateVec;
	}
	if (abs(subAngle) > abs(maxAngle)) {
		walkReturnFlag = true;
	}
}

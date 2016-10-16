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
	concussion = 0.0f;
	damageFlag = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Sandbag::Start(){
}

//���t���[���Ă΂�܂�
void Sandbag::Update(){
	vec = XMVectorZero();
	if (damageFlag) {
		concussion += Hx::DeltaTime()->GetDeltaTime();
		if (concussion > 3.0f) {
			damageFlag = false;
		}
		vec += XMVectorSet(0, 500, 0, 0);
	}


	XMVECTOR playerPos = Hx::FindActor("new Sphere")->mTransform->WorldPosition();
	auto mat = gameObject->mTransform->Children().front()->GetComponent<MaterialComponent>();
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!mat) return;

	auto forward = XMVector3Normalize(gameObject->mTransform->Forward());
	auto playerVec = playerPos - gameObject->mTransform->Position();
	forward.y = 0;
	playerVec.y = 0;
	float view = acos(XMVector3Dot(forward, XMVector3Normalize(playerVec)).x);
	
	if (XMVector3Length(playerVec).x < trackingRange && view / 3.14f * 180.0f < trackingAngle) {
		mat->SetAlbedoColor(XMFLOAT4(1, 0, 0, 1));
		vec += forward * trackingSpeed;
		auto cross = XMVector3Normalize(XMVector3Cross(forward, XMVector3Normalize(playerVec)));

		auto qua = gameObject->mTransform->Quaternion();
		gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, view)));
	}
	else {
		mat->SetAlbedoColor(XMFLOAT4(0, 1, 0, 1));

		if (!cc)return;

		//�ǂɓ������ďC���������܂��ǂɓ������Ă��邩���ׂ�
		if (walkReturnFlag) {
			changeVec = false;
			subAngle = 0.0f;
		}

		if (!changeVec) {
			Walk();
		}
		else {
			WallHit();
		}
		//Angle�̃N�����v
		if (angle > 360)angle -= 360.0f;
		else if (angle < 0)angle += 360.0f;

		//if (Input::Trigger(KeyCoord::Key_D)) {
		//	Damage(1);
		//}

		//�l��Set
		gameObject->mTransform->Rotate(XMVectorSet(0, angle * 3.14f / 180.0f, 0, 0));
	}

	//�d��
	if (!cc->IsGround()) {
		vec += mGravity;
	}

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
	damageFlag = true;
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

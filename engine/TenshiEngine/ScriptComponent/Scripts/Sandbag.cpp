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
		if (concussion > concussionTime) {
			damageFlag = false;
		}
		vec += XMVectorSet(0, jumpPower, 0, 0);
	}

	AnimLerp();

	if (!player)return;
	XMVECTOR playerPos = player->mTransform->WorldPosition();
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();

	auto forward = XMVector3Normalize(gameObject->mTransform->Forward());
	auto playerVec = playerPos - gameObject->mTransform->Position();
	forward.y = 0;
	playerVec.y = 0;
	float view = acos(XMVector3Dot(forward, XMVector3Normalize(playerVec)).x);
	
	if (XMVector3Length(playerVec).x < trackingRange && view / 3.14f * 180.0f < trackingAngle) {
		vec += forward * trackingSpeed;
		auto cross = XMVector3Normalize(XMVector3Cross(forward, XMVector3Normalize(playerVec)));

		auto qua = gameObject->mTransform->Quaternion();
		gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, view)));
	}
	else {

		if (!cc)return;

		//�ǂɓ������ďC���������܂��ǂɓ������Ă��邩���ׂ�
		if (walkReturnFlag) {
			changeVec = false;
			subAngle = 0.0f;
		}

		if (!changeVec) {
			AnimChange(1, 5.0f);
			Walk();
		}
		else {
			AnimChange(0, 5.0f);
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
	concussion = 0.0f;
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

void Sandbag::AnimChange(int id,float speed)
{
	if (id != animparam.beforeAnimId) {
		if (!animparam.animLerpFlag) {
			animparam.afterAnimId = id;
			animparam.animLerpFlag = true;
			animparam.lerpSpeed = speed;
		}
	}
}

void Sandbag::AnimLerp()
{
	if (animparam.animLerpFlag) {
		if (!modelObject)return;
		auto anim = modelObject->GetComponent<AnimationComponent>();
		if (!anim)return;

		animparam.nowLerpTime += animparam.lerpSpeed * Hx::DeltaTime()->GetDeltaTime();

		bool endFlag = false;
		if (animparam.nowLerpTime >= 1.0f) {
			animparam.nowLerpTime = 1.0f;
			endFlag = true;
		}

		AnimeParam ap;
		ap = anim->GetAnimetionParam(animparam.beforeAnimId);
		ap.mWeight = 1.0f - animparam.nowLerpTime;
		anim->SetAnimetionParam(animparam.beforeAnimId, ap);

		ap = anim->GetAnimetionParam(animparam.afterAnimId);
		ap.mWeight = animparam.nowLerpTime;
		anim->SetAnimetionParam(animparam.afterAnimId, ap);

		if (endFlag) {
			animparam.beforeAnimId = animparam.afterAnimId;
			animparam.nowLerpTime = 0.0f;
			animparam.animLerpFlag = false;
		}
	}
}

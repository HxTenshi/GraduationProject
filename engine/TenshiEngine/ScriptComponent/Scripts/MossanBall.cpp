#include "MossanBall.h"
#include "h_standard.h"
#include "h_component.h"
//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void MossanBall::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void MossanBall::Start(){
	count_explode_time = 0.0f;
	timer = 0.0f;
	enable = false;
}

//���t���[���Ă΂�܂�
void MossanBall::Update(){

	timer += Hx::DeltaTime()->GetDeltaTime();
	if (timer >= instance_time && !enable) {
		enable = true;
		gameObject->GetComponent<MeshDrawComponent>()->Enable();
	}
	if(enable){
		ExplodeTimer();
		ExplodeFunc();
		Shooter();
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void MossanBall::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void MossanBall::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void MossanBall::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void MossanBall::OnCollideExit(GameObject target){
	(void)target;
}

void MossanBall::ExplodeTimer()
{
	count_explode_time += Hx::DeltaTime()->GetDeltaTime();
}

void MossanBall::ExplodeFunc()
{
	if (count_explode_time >= explodeTime) {
		Hx::DestroyObject(gameObject);
	}
}

void MossanBall::SetDir(XMVECTOR dir)
{
	direction = dir;
}

void MossanBall::Shooter()
{
	auto pos = gameObject->mTransform->WorldPosition();
	auto dir = XMVector3Normalize(direction);
	pos += dir*speed*Hx::DeltaTime()->GetDeltaTime();
	gameObject->mTransform->WorldPosition(pos);
}

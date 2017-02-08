#include "MossanBall.h"
#include "h_standard.h"
#include "h_component.h"
//生成時に呼ばれます（エディター中も呼ばれます）
void MossanBall::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void MossanBall::Start(){
	count_explode_time = 0.0f;
	timer = 0.0f;
	enable = false;
}

//毎フレーム呼ばれます
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

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void MossanBall::Finish(){

}

//コライダーとのヒット時に呼ばれます
void MossanBall::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void MossanBall::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
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

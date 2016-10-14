#include "Sandbag.h"

#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void Sandbag::Initialize(){
	mGravity = XMVectorSet(0, -9.81f, 0, 1);
	angle = 0.0f;
	changeVec = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Sandbag::Start(){
}

//毎フレーム呼ばれます
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

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Sandbag::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Sandbag::OnCollideBegin(GameObject target) {
	(void)target;	
}

//コライダーとのヒット中に呼ばれます
void Sandbag::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void Sandbag::OnCollideExit(GameObject target){
	(void)target;
}

void Sandbag::SetHitWall(bool hitWall)
{
	changeVec = true;
}

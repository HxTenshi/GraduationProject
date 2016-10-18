#include "DemoWeapon.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void DemoWeapon::Initialize(){
	mShoot = false;
	mThrowVector = XMVectorSet(0, 0, 0, 0);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void DemoWeapon::Start(){

}

//毎フレーム呼ばれます
void DemoWeapon::Update(){
	if(mShoot)gameObject->mTransform->WorldPosition(gameObject->mTransform->WorldPosition() + mThrowVector);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void DemoWeapon::Finish(){

}

//コライダーとのヒット時に呼ばれます
void DemoWeapon::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void DemoWeapon::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void DemoWeapon::OnCollideExit(GameObject target){
	(void)target;
}

void DemoWeapon::SetReleaseVector(GameObject player,XMVECTOR position)
{
	//なぜかアクセスエラーになる
	Hx::Debug()->Log(std::to_string(gameObject->mTransform->Position().x));

	//float speed = 10;
	//auto front = player->mTransform->Forward();
	//front = XMVector3Normalize(front);
	//XMVECTOR power = front * speed * Hx::DeltaTime()->GetDeltaTime();
	//XMVECTOR result = power + player->mTransform->Position();

	//mThrowVector = power * gameObject->mTransform->Position();
}

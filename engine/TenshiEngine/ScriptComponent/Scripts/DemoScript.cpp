#include "DemoScript.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"
# include "DemoWeapon.h"

DemoScript::DemoScript()
{

}

void DemoScript::Initialize()
{

}
void DemoScript::Start()
{
	mContainer = SearchChildren("WeaponContainer");
	mWeaponContainer = mContainer->GetScript<WeaponContainer>();

	Hx::Debug()->Log("午後はここから");
	Hx::Debug()->Log("DemoWeaponを取得出来ていたら、それを投げる Pysicx無しで。　着弾地点に若干埋め込む感じで。");

	Hx::Debug()->Log("投げた後、刃先を若干埋め込むならDemoWeaponの子に刃先となる空のオブジェクトを設定する。それを見て投げる物のRotaを設定しておく。");

	Hx::Debug()->Log("DemoWeaponの座標を取得>。そこまで自分を移動させる。");
}
void DemoScript::Update()
{
	GameObject camera = Hx::FindActor("Camera");


	float speed = 1;
	XMVECTOR position = gameObject->mTransform->WorldPosition();
	if (Input::Down(KeyCoord::Key_A)) {
		position += XMVectorSet(-speed, 0, 0, 0) * Hx::DeltaTime()->GetDeltaTime();
	}
	if (Input::Down(KeyCoord::Key_D)) {
		position += XMVectorSet(speed, 0, 0, 0) * Hx::DeltaTime()->GetDeltaTime();
	}

	//武器の投擲(正面に)
	if (Input::Down(KeyCoord::Key_E)) {
		Throw();
	}

	if (Input::Down(KeyCoord::Key_Q)) {
		TargetMove();
	}


	//仮プレイヤーの移動
	gameObject->mTransform->WorldPosition(position);
}
void DemoScript::Finish()
{

}
void DemoScript::OnCollideBegin(GameObject target)
{
	Hx::Debug()->Log("当たったもの : "+target->Name());
	//当たったものを子にする
	XMVECTOR up = XMVectorSet(1,2,1,0);
	target->mTransform->WorldPosition(gameObject->mTransform->WorldPosition()+up);
	Hx::Debug()->Log("今は親子関係外している");
	//target->mTransform->SetParent(gameObject);
	mWeaponContainer->AddWeapon(target);
}
void DemoScript::OnCollideEnter(GameObject target)
{

}
void DemoScript::OnCollideExit(GameObject target)
{

}

//武器の方向へ飛んでいく
void DemoScript::TargetMove()
{
	//auto weapon = mWeaponContainer->GetWeapon()->mTransform->WorldPosition();
	//auto myposition = gameObject->mTransform->WorldPosition();
	//float dx = weapon.x - myposition.x;
	//float dz = weapon.z - myposition.z;
	//float rad = std::atan2(dz, dx);
	//float angle = rad * 360 / 3.14f * 2 - 90;
	//gameObject->mTransform->Rotate(XMVectorSet(0,angle,0,0));
	//Hx::Debug()->Log(std::to_string(angle));
	//XMVECTOR power = 100 * XMVector3Normalize(gameObject->mTransform->Forward());
	//Hx::Debug()->Log("x"+std::to_string(power.x));
	//Hx::Debug()->Log("y"+std::to_string(power.y));
	//Hx::Debug()->Log("z"+std::to_string(power.z));
	//gameObject->mTransform->AddForce(power);
}

XMVECTOR DemoScript::GetThrowTarget()
{
	GameObject target = Hx::FindActor("Point");
	return XMVECTOR();
}

void DemoScript::Throw()
{
	//mWeaponContainer->gameObject->GetScript<DemoWeapon>()->SetReleaseVector(gameObject, mWeaponContainer->GetWeapon()->mTransform->Position());
}



//名前判定して子を取得する
GameObject DemoScript::SearchChildren(std::string name)
{
	GameObject targer;
	for(auto i : gameObject->mTransform->Children())
	{
		if (name == i->Name())targer = i;
	}
	return targer;
}

#include "WeaponControl.h"
#include "h_component.h"
#include "h_standard.h"
# include "Game\Component\BoneMirrorComponent.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void WeaponControl::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WeaponControl::Start(){

}

//毎フレーム呼ばれます
void WeaponControl::Update(){

	if (isHit)
	{
		mTime += Hx::DeltaTime()->GetDeltaTime();
		if (mTime > mHitTime) {
			mTime = 0;
			isHit = false;
			Hx::Debug()->Log("投げた後の移動可能な時間を超過しました。");
		}
	}

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void WeaponControl::Finish(){

}

//コライダーとのヒット時に呼ばれます
void WeaponControl::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void WeaponControl::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void WeaponControl::OnCollideExit(GameObject target){
	(void)target;
}

void WeaponControl::HitActor(GameObject target,GameObject weapon)
{
	Hx::Debug()->Log("当たっているはず");
	Hx::Debug()->Log(" : "+std::to_string(target->GetLayer()));
	Hx::Debug()->Log(" : " + target->Name());
	if (target->GetLayer() == 3){
		Hx::Debug()->Log("頭に");
		SearchEnemyBone(target, weapon, "Head");
	}
	else if (target->GetLayer() == 12) {
		Hx::Debug()->Log("体に");
		SearchEnemyBone(target, weapon, "Spine");
	}

}

//
void WeaponControl::SearchEnemyBone(GameObject target,GameObject weapon,std::string name)
{
		//ここで対象の敵の紐付け
		auto mirrer = weapon->GetComponent<BoneMirrorComponent>();
		std::list<GameObject> targetObject;
		if (target->GetLayer() == 3){
			targetObject = target->mTransform->Children();
		}
		else if (target->GetLayer() == 12) {
			targetObject = target->mTransform->GetParent()->mTransform->Children();
		}

		for (auto i : targetObject) {
			if (i->GetLayer() == 10)target = i;
		}

		mirrer->ChangeTargetBone(target);
		mirrer->Enable();

		auto vector = mirrer->GetBoneNames();
		int id = 0;
		for (auto name : vector)
		{
			if (name == name)
			{
				break;
			}
			id++;
		}

		//追従する
		mirrer->SetTargetBoneID(id);
		isHit = true;
}

//頭に当たったか?
bool WeaponControl::IsHitHead(GameObject target)
{
	//for (GameObject i : target->mTransform->Children())
	//{
	//	if (i->GetLayer() == 12)return true;
	//}
	if (target->GetLayer() == 12)return true;
	return false;
}

bool WeaponControl::IsHit()
{
	return isHit;
}

void WeaponControl::DeleteHitPoint()
{
	isHit = false;
	mTime = 0;
}

void WeaponControl::HitStage(GameObject target, GameObject weapon)
{
	//Weaponの止まる処理を追加する
	auto physics = weapon->GetScript<PhysXComponent>();
	//Hx::Debug()->Log(weapon->Name());
	//Hx::Debug()->Log(std::to_string(physics->gameObject));
	if (physics)
	{
		Hx::Debug()->Log("キネマてぃっく");
		physics->SetKinematic(true);
	}

}
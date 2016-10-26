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
	IsEnemy(target,weapon);
}


//
void WeaponControl::IsEnemy(GameObject target,GameObject weapon)
{
	//敵に当たったら
	if (target->GetLayer() == 3)
	{
		//ここで対象の敵の紐付け
		auto mirrer = weapon->GetComponent<BoneMirrorComponent>();
		Hx::Debug()->Log("当たったSandBagの子から特例のLayer(10)を検出してそのオブジェクトのBoneを見ている");
		auto targetObject = target->mTransform->Children();
		for (auto i : targetObject) {
			if (i->GetLayer() == 10)target = i;
		}
		mirrer->ChangeTargetBone(target);
		mirrer->Enable();

		auto vector = mirrer->GetBoneNames();
		int id = 0;
		for (auto name : vector)
		{
			if (name == "Head")
			{
				break;
			}
			id++;
		}

		//追従する
		mirrer->SetTargetBoneID(id);
		
		//刺さる
		//weapon->mTransform->SetParent(target);
		//weapon->mTransform->WorldPosition(target->mTransform->WorldPosition()+XMVectorSet(0,2,0,0));
		//武器の回転(刺さる方向)
		//weapon->mTransform->DegreeRotate(XMVectorSet(90,0,0,0));
		//weapon->GetComponent<PhysXComponent>()->SetKinematic(false);
		//weapon->
	}
}

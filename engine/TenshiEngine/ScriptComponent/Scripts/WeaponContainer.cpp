#include "WeaponContainer.h"
#include "../h_standard.h"
#include "../h_component.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void WeaponContainer::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WeaponContainer::Start(){
	mWeapn = mGetWeapon->GetScript<GetWeapon>();
}

//毎フレーム呼ばれます
void WeaponContainer::Update(){
	
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void WeaponContainer::Finish(){

}

//コライダーとのヒット時に呼ばれます
void WeaponContainer::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void WeaponContainer::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void WeaponContainer::OnCollideExit(GameObject target){
	(void)target;
}

//取得済み武器の取得
//GameObject WeaponContainer::GetWeapon()
//{
//	return mWeapons.front();
//}
//武器の取得
void WeaponContainer::AddWeapon(GameObject target)
{
	mWeapons.push_back(target);
}



void WeaponContainer::Release(GameObject weapon)
{
	mWeapons.remove(weapon);
}

//子の武器を取得する。
void WeaponContainer::GetChildren()
{
	mWeapons = gameObject->mTransform->Children();
}

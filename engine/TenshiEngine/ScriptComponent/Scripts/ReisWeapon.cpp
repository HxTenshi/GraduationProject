#include "ReisWeapon.h"
#include "UniqueObject.h"
#include "reis.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void ReisWeapon::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ReisWeapon::Start(){

}

//毎フレーム呼ばれます
void ReisWeapon::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ReisWeapon::Finish(){

}

//コライダーとのヒット時に呼ばれます
void ReisWeapon::OnCollideBegin(GameObject target){
	(void)target;


}

//コライダーとのヒット中に呼ばれます
void ReisWeapon::OnCollideEnter(GameObject target){
	if (UniqueObject::IsPlayer(target) && m_Reis) {
		if (auto r = m_Reis->GetScript<reis>()) {
			r->WeaponCallHit();
		}
	}
}

//コライダーとのロスト時に呼ばれます
void ReisWeapon::OnCollideExit(GameObject target){
	(void)target;
}
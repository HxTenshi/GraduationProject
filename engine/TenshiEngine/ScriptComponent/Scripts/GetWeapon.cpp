#include "GetWeapon.h"
#include "h_standard.h"
#include "WeaponHand.h"
#include "Weapon.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void GetWeapon::Initialize(){
	m_ExClusionWeapon = NULL;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void GetWeapon::Start(){

}

//毎フレーム呼ばれます
void GetWeapon::Update(){
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void GetWeapon::Finish(){

}

//コライダーとのヒット時に呼ばれます
void GetWeapon::OnCollideBegin(GameObject target){
	if (!target)return;
	if (!target->GetScript<Weapon>()) return;
	m_WeaponList.push_back(target);
}

//コライダーとのヒット中に呼ばれます
void GetWeapon::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void GetWeapon::OnCollideExit(GameObject target){
	if (!target) return;
	if (!target->GetScript<Weapon>()) return;
	m_WeaponList.remove_if([&](auto o) {
		return o == target || !o;});
}

GameObject GetWeapon::GetMinWeapon(){
	auto pos = gameObject->mTransform->WorldPosition();
	float lowL = 9999999.0f;
	GameObject lowObj = NULL;
	for (auto weapon : m_WeaponList) {
		if (!weapon) continue;
		if (weapon == m_ExClusionWeapon) {
			continue;
		}
		auto l = XMVector3Length(pos - weapon->mTransform->WorldPosition()).x;
		if (l < lowL) {
			lowObj = weapon;
			lowL = l;
		}
		
	}
	return lowObj;
}

GameObject GetWeapon::GetPointMinWeapon(GameObject currentTarget, MinVect::ENum minVect){
	if (!currentTarget)return NULL;
	XMVECTOR camvec;
	if (m_Camera) {
		camvec = m_Camera->mTransform->Forward();
	}

	XMVECTOR vect = XMVector3Normalize(currentTarget->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition());
	float lowL = 9999999.0f;
	GameObject lowObj = NULL;

	auto mat = XMMatrixLookToLH(XMVectorZero(), vect, XMVectorSet(0, 1, 0, 1));
	mat = XMMatrixTranspose(mat);
	XMVECTOR Null;
	mat = XMMatrixInverse(&Null, mat);

	auto temp = XMMatrixMultiply(XMMatrixTranslationFromVector(currentTarget->mTransform->WorldPosition()), mat);
	auto CurrentTargetPos = temp.r[3];

	for (auto weapon : m_WeaponList) {
		if (!weapon) continue;
		if (weapon == m_ExClusionWeapon) {
			continue;
		}	
		auto posmat = XMMatrixMultiply(XMMatrixTranslationFromVector(weapon->mTransform->WorldPosition()), mat);
		float l = abs(posmat.r[3].x - CurrentTargetPos.x);

		XMVECTOR vect2 = XMVector3Normalize(weapon->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition());
		if (m_Camera) {
			if (XMVector3Dot(camvec, vect2).x < 0)continue;
		}

		if (minVect == MinVect::left) {

			if (posmat.r[3].x < CurrentTargetPos.x) {
				if (l < lowL) {
					lowObj = weapon;
					lowL = l;
				}
			}
		}
		else if (minVect == MinVect::right) {
			if (posmat.r[3].x > CurrentTargetPos.x) {
				if (l < lowL) {
					lowObj = weapon;
					lowL = l;
				}
			}
		}
	}
	return lowObj;
}

void GetWeapon::SetExClusionWeapon(GameObject weapon){
	m_ExClusionWeapon = weapon;
}

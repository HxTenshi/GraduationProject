#include "WeaponHand.h"
#include "h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void WeaponHand::Initialize(){
	m_ActionFree = false;
	m_Wave = 0.0f;
	mWeapon = NULL;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WeaponHand::Start(){

}

//毎フレーム呼ばれます
void WeaponHand::Update(){
	if (!mWeapon) {
		return;
	}
	if (m_NowGetAction) {

		auto time = Hx::DeltaTime()->GetDeltaTime();

		auto weapon_pos = mWeapon->mTransform->WorldPosition();
		auto weapon_rot = mWeapon->mTransform->WorldQuaternion();
		weapon_pos.y -= m_Wave;

		auto end_pos = gameObject->mTransform->WorldPosition();
		auto end_rot = gameObject->mTransform->WorldQuaternion();
		
		auto dis = end_pos - weapon_pos;
		auto v = XMVector3Normalize(dis);
		float l = XMVector3Length(dis).x;
		if (m_GetPosDistance) {
			float d = XMVector3Length(XMVectorSet(dis.x,0,dis.z,1)).x;
			m_Wave = sin(d / m_GetPosDistance * XM_PI) * m_WaveHeight;
			weapon_pos.y += m_Wave;
		}
		weapon_pos += v * m_GetSpeed*time;
		if (l < m_GetSpeed*time) {
			mWeapon->mTransform->SetParent(gameObject);
			mWeapon->mTransform->Position(XMVectorZero());
			mWeapon->mTransform->Rotate(XMVectorZero());

			m_ActionFree = true;
			m_NowGetAction = false;
		}
		else {
			mWeapon->mTransform->WorldPosition(weapon_pos);

			auto left = XMVector3Cross(v, XMVectorSet(0, 1, 0, 1));
			auto quat = XMQuaternionRotationAxis(left, m_GetRotSpeed*time);
			mWeapon->mTransform->WorldQuaternion(XMQuaternionMultiply(weapon_rot, quat));
		}

	}

	if (!m_ActionFree) {
		return;
	}

	{
		mWeapon->mTransform->SetParent(gameObject);
		mWeapon->mTransform->Position(XMVectorZero());
		mWeapon->mTransform->Rotate(XMVectorZero());
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void WeaponHand::Finish(){

}

//コライダーとのヒット時に呼ばれます
void WeaponHand::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void WeaponHand::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void WeaponHand::OnCollideExit(GameObject target){
	(void)target;
}

GameObject WeaponHand::GetHandWeapon()
{
	return mWeapon;
}

void WeaponHand::SetWeapon(GameObject weapon, const Weapon::HitCollbackType& collback)
{
	if (mWeapon)return;
	//if (mWeapon) {
	//	Hx::DestroyObject(mWeapon);
	//}
	mWeapon = weapon;
	if (!mWeapon)return;
	m_ActionFree = false;
	m_NowGetAction = true;
	m_Wave = 0.0f;
	auto pos1 = gameObject->mTransform->WorldPosition();
	auto pos2 = mWeapon->mTransform->WorldPosition();
	auto v = pos1 - pos2;
	v.y = 0.0f;
	m_GetPosDistance = XMVector3Length(v).x;
	mWeapon->mTransform->SetParent(Hx::GetRootActor());
	mWeapon->mTransform->WorldPosition(pos2);

	if (auto scr = mWeapon->GetScript<Weapon>()) {
		scr->GetWeapon();
		scr->SetHitCollback(collback);
	}
}

void WeaponHand::ThrowAway()
{
	if (!mWeapon || !m_ActionFree)return;
	if (auto scr = mWeapon->GetScript<Weapon>()) {
		//auto v = gameObject->mTransform->Forward();
		//scr->ThrowAway(v);
		mWeapon = NULL;
		scr->ThrowAway();
		m_ActionFree = false;
	}
}

void WeaponHand::ThrowAway(GameObject target,bool isMove)
{
	if (!mWeapon || !m_ActionFree)return;
	//ここにクラスで計算したものを代入
	//GameObjectを引数に角度とか出せるもの
	XMVECTOR log = target->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
	float rad = std::atan2(log.z, log.x);
	float angle = rad * 180 / 3.14f;
	if (angle > 360)angle -= 360.0f;
	else if (angle < 0)angle += 360.0f;
	Hx::Debug()->Log("Y : "+std::to_string(angle));
	GameObject character = Hx::FindActor("キャラクター");
	character->mTransform->DegreeRotate(XMVectorSet(0, angle, 0, 0));

	mWeapon->GetComponent<PhysXComponent>()->SetGravity(false);

	XMVECTOR targetVector = XMVector3Normalize(log);
	float power = 1;
	character->mTransform->AddForce(targetVector * 30,ForceMode::eIMPULSE);
	if (auto scr = mWeapon->GetScript<Weapon>()) {
		mWeapon = NULL;
		scr->ThrowAway(targetVector * power);
		m_ActionFree = false;
	}
}

void WeaponHand::ThrowAway(XMVECTOR vector)
{
	if (!mWeapon || !m_ActionFree)return;
	float power = 1;
	if (auto scr = mWeapon->GetScript<Weapon>()) {
		mWeapon = NULL;
		scr->ThrowAway(vector * power);
		m_ActionFree = false;
	}
}
#include "WeaponHand.h"
#include "h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void WeaponHand::Initialize(){
	m_AttackTime = 0.0f;
	m_IsGuard = false;
	m_ActionFree = false;
	m_Wave = 0.0f;
	mWeapon = NULL;

	m_AttackFunction = [](){};
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WeaponHand::Start(){

}

//毎フレーム呼ばれます
void WeaponHand::Update(){
	if (!mWeapon) {
		m_AttackTime = 0.0f;

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

	if (m_IsGuard) {
		if(m_GuardPos)
			mWeapon->mTransform->SetParent(m_GuardPos);
	}
	else {
		mWeapon->mTransform->SetParent(gameObject);
		mWeapon->mTransform->Position(XMVectorZero());
		mWeapon->mTransform->Rotate(XMVectorZero());

		m_AttackFunction();
	}
	m_IsGuard = false;
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

void WeaponHand::LowAttack_1()
{
	m_AttackTime = 1.0f;
	m_AttackFunction = [&]() {
		if (m_AttackTime > 0.0f) {
			m_AttackTime -= Hx::DeltaTime()->GetDeltaTime();
			m_AttackTime = max(m_AttackTime, 0.0f);
			mWeapon->mTransform->Rotate(XMVectorSet(m_AttackTime * 10.0f, 0, 0, 1));
		}
	};
}
void WeaponHand::LowAttack_2()
{
	m_AttackTime = 1.0f;
	m_AttackFunction = [&]() {
		if (m_AttackTime > 0.0f) {
			m_AttackTime -= Hx::DeltaTime()->GetDeltaTime();
			m_AttackTime = max(m_AttackTime, 0.0f);
			mWeapon->mTransform->Rotate(XMVectorSet(0, 0, m_AttackTime * 10.0f, 1));
		}
	};
}
void WeaponHand::LowAttack_3()
{
	m_AttackTime = 1.0f;
	m_AttackFunction = [&]() {
		if (m_AttackTime > 0.0f) {
			m_AttackTime -= Hx::DeltaTime()->GetDeltaTime();
			m_AttackTime = max(m_AttackTime, 0.0f);
			mWeapon->mTransform->Rotate(XMVectorSet(m_AttackTime * 40.0f, 0, 0, 1));
		}
	};
}

void WeaponHand::HighAttack_1()
{
	m_AttackTime = 1.0f;
	m_AttackFunction = [&]() {
		if (m_AttackTime > 0.0f) {
			m_AttackTime -= Hx::DeltaTime()->GetDeltaTime();
			m_AttackTime = max(m_AttackTime, 0.0f);

			auto pos = sin(m_AttackTime * XM_PI) * 1.0f;
			mWeapon->mTransform->Position(XMVectorSet(0,0,pos,1));
			mWeapon->mTransform->Rotate(XMVectorSet(m_AttackTime * 40.0f, 0, 0, 1));
		}
	};
}

void WeaponHand::HighAttack_2()
{
	m_AttackTime = 1.0f;
	m_AttackFunction = [&]() {
		if (m_AttackTime > 0.0f) {
			m_AttackTime -= Hx::DeltaTime()->GetDeltaTime();
			m_AttackTime = max(m_AttackTime, 0.0f);

			auto pos1 = sin(m_AttackTime * XM_PI) * 1.0f;
			auto pos2 = (sin(m_AttackTime * XM_PI * 2)) * 2.0f;
			mWeapon->mTransform->Position(XMVectorSet(pos2, 0, pos1,1));
			mWeapon->mTransform->Rotate(XMVectorSet(0, 0, m_AttackTime * 40.0f, 1));
		}
	};
}

void WeaponHand::FloatLowAttack_1()
{
	m_AttackTime = 1.0f;
	m_AttackFunction = [&]() {
		if (m_AttackTime > 0.0f) {
			m_AttackTime -= Hx::DeltaTime()->GetDeltaTime();
			m_AttackTime = max(m_AttackTime, 0.0f);
			mWeapon->mTransform->Rotate(XMVectorSet(0, 0, m_AttackTime * 40.0f, 1));
		}
	};
}

void WeaponHand::SpecialAttack()
{
	m_AttackTime = 3.0f;
	m_AttackFunction = [&]() {
		if (m_AttackTime > 0.0f) {
			m_AttackTime -= Hx::DeltaTime()->GetDeltaTime();
			m_AttackTime = max(m_AttackTime, 0.0f);
			mWeapon->mTransform->Rotate(XMVectorSet(0, 0, m_AttackTime * 40.0f, 1));
		}
	};
}


void WeaponHand::Guard()
{
	m_IsGuard = true;

}

void WeaponHand::ThrowAway()
{
	if (!mWeapon || !m_ActionFree)return;
	if (auto scr = mWeapon->GetScript<Weapon>()) {
		//auto v = gameObject->mTransform->Forward();
		//scr->ThrowAway(v);
		mWeapon = NULL;
		scr->ThrowAway();
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
	}
}

void WeaponHand::ThrowAway(XMVECTOR vector)
{
	if (!mWeapon || !m_ActionFree)return;
	float power = 1;
	if (auto scr = mWeapon->GetScript<Weapon>()) {
		mWeapon = NULL;
		scr->ThrowAway(vector * power);
	}
}